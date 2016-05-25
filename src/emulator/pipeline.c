/* Module  : pipeline
 *
 * Usage   : A top-level module which initialises arm11 can call emulate() to
 *           begin program execution. An instruction handler can read the
 *           program counter.
 *
 * Authors : Tencho Tenev
 */

#include "pipeline.h"
#include <stdbool.h>
#include <assert.h>
#include "arm11.h"
#include "util/cpsr_flags.h"
#include "branch.h"
#include "data_processing.h"
#include "multiply.h"
#include "sdt.h"
#include "printer.h"

#define ASSERT_ADDRESS(address) assert(address >= 0 && address < MEMORY_SIZE)

/* Pipeline state */

static enum status current = initial;

static void (*handler)(union decoded_instr * );

static bool can_decode, can_execute, is_branch;

/* End of Pipeline state*/

/* Helper functions */
static void (*decode(union instruction * ))(union decoded_instr * );

static void halt(union decoded_instr * );

static void nop(union decoded_instr * );

static bool check_cond(uint32_t cond);

/*
 * Function : emulate
 * Usage    : emulate(0x00000000)
 * ------------------------------
 * This procedure takes an address (usually 0) and emulates ARM11 3-stage
 * pipeline execution using the arm11 interface. The actual emulation
 * reproduces the fetch-decode-execute cycle. PC is guaranteed to be 8 bytes
 * greater than the address of an executed instruction.
 *
 * The value at the given address is used to set the program counter.
 *
 * PRE  : The arm11 module is initialised.
 *        pc is a valid address (0x0000****)
 *        status is initial
 *
 * POST : Returns 0 if the program terminated successfully meaning that the
 *        state of arm11 registers matches the termination state.
 *
 *        Returns non-zero in case of errors which prevented the program
 *        execution to reach a termination instruction.
 */
int emulate(uint32_t pc_address) {
    if (current != initial) {
        return EXIT_FAILURE;
    }

    union instruction *fetched;
    union decoded_instr *decoded;

    current = running;
    can_decode = can_execute = is_branch = false;

    em_set_pc(pc_address);

    while (current == running) {

        // Execute
        if (can_execute) {
            handler(decoded);

            if (is_branch) {
                /* This must be a branch so the next cycle will skip execution
                 * and this cycle will skip decode
                 */
                can_decode = can_execute = is_branch = false;
            }
        }

        // Decode
        if (can_decode) {
            /* decode will set is_branch to true if this is a branch
             * instruction because it will be executed on the next cycle when
             * no decoding should occur
             */
            handler = decode(fetched);
            decoded = &(fetched->decoded);

            // Enable execution after decoding
            can_execute = true;
        }

        // Fetch
        fetched = get_instr(em_get_pc());

        // Update PC
        em_acc_pc(WORD_SIZE);

        // Enable decode on next cycle after branch (or initially)
        if (!can_execute) {
            can_decode = true;
        }
    }

    // Discard last increment
    em_acc_pc(-WORD_SIZE);

    return EXIT_SUCCESS;
}

/*
 * Function : reset
 * ----------------
 * Sets the status to initial. If emulate is running, it will return after
 * completing the current cycle.
 */
void em_reset() {
    current = initial;
}

/*
 * Function : get_status
 * Usage    : status emu_status = get_status()
 * ---------------------------------
 * Returns one of initial, running, or terminated
 *      initial iff emulate has not been called
 *      running iff emulate was called and has not returned
 *      terminated iff emulate was called and returned
 */
enum status em_get_status() {
    return current;
}

/*
 * Function : get_pc
 * Usage    : uint32_t pc = get_pc()
 * ---------------------------------
 * Provides read access to the program counter. Note that the program counter
 * is always exactly 8 bytes greater than the currently executed instruction
 */
uint32_t em_get_pc(void) {
    return get_register(PC_INDEX);
}

/*
 * Function : em_set_pc
 * Usage    : em_set_pc(em_get_pc() + offset);
 * -------------------------------------
 * Set program counter (used when branching)
 */
 void em_set_pc(uint32_t new_pc) {
     ASSERT_ADDRESS(new_pc);
     set_register(PC_INDEX, new_pc);
 }

 /*
  * Function : em_acc_pc
  * Usage    : em_acc_pc(offset);
  * -------------------------------------
  * Accumulate program counter (used when branching)
  */
 void em_acc_pc(uint32_t offset) {
     set_register(PC_INDEX, get_register(PC_INDEX) + offset);
 }

/*
 * Function : decode
 * Usage    : handler = decode(fetched)
 * -----------------
 * Takes a pointer to an instruction and returns a function pointer to the
 * correct handler for the instruction type
 *
 * If the fetched instruction is branch also sets is_branch to true
 */

/* The 2 bits that come after Cond */
enum instr_id {
    DP_MULT_ID,
    SDT_ID,
    BRANCH_ID
};

static void (*decode(union instruction *fetched))(union decoded_instr * ) {
    // TODO: Implement actual decoding
    // return halt;

    if(fetched->bin == 0) {
      return halt;
    }

    bool cond = check_cond(fetched->decoded.dp.cond);
    if (cond) {
        switch (fetched->decoded.dp._id) {

        case DP_MULT_ID: {
            if (fetched->decoded.dp.imm_op == 1) {
                return dp_exec;
            }

            if (!fetched->decoded.mul._mul4) {
                return dp_exec;
            }

            if (!fetched->decoded.mul._mul7) {
                return dp_exec;
            }

            return mul_exec;
        }

        case SDT_ID: {
          return sdt_exec;
        }

        case BRANCH_ID: {
          is_branch = true;
          return br_exec;
        }

        default: {
          assert(false); // Invalid instruction
        }
        }
    } else {
      return nop;
    }

}

/* Termination handler */
static void halt(union decoded_instr *instr) {
    current = terminated;
}

/*
 * A do nothing instruction used when condition fails
 */
static void nop(union decoded_instr *instr) {
  return;
}

enum condition {
  eq = 0, // Z
  ne = 1, // !Z
  ge = 10, // N == V
  lt = 11, // N != V
  gt = 12, // !Z && N == V
  le = 13, // Z || N != V
  al = 14  // true
};

static bool check_cond(uint32_t cond) {
    // TODO: check cond with cpsr
    switch(cond) {
      case eq: return get_zflag;
      case ne: return !get_zflag;
      case ge: return get_nflag == get_vflag;
      case lt: return get_nflag != get_vflag;
      case gt: return !get_zflag && (get_nflag == get_vflag);
      case le: return get_zflag || (get_nflag != get_vflag);
      case al: return true;
      default: return false;
    }
}
