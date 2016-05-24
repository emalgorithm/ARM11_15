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

/* Pipeline state */

static enum status current = initial;

static uint32_t pc;

static union instruction *fetched, *decoded;

static void (*handler)(union instruction * );

bool can_decode, can_execute, is_branch;

/* End of Pipeline state*/

/* Helper functions */
static void (*decode(union instruction * ))(union instruction * );

static void halt(union instruction * );

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
        return 1;
    }

    current = running;
    can_decode = can_execute = is_branch = false;

    pc = get_word(pc_address);

    while (current == running) {

        // Execute
        if (can_execute) {
            handler(decoded);

            if (is_branch) {
                /* This must be a branch so the next cycle will skip execution
                 * and this cycle will skip decode
                 */
                can_decode = can_execute = false;
            }
        }

        // Decode
        if (can_decode) {
            /* decode will set is_branch to true if this is a branch
             * instruction because it will be executed on the next cycle when
             * no decoding should occur
             */
            handler = decode(fetched);
            decoded = fetched;

            // Enable execution after decoding
            can_execute = true;
        }

        // Fetch
        fetched = get_instr(pc);

        // Update PC
        pc += WORD_SIZE;

        // Enable decode on next cycle after branch (or initially)
        if (!can_execute) {
            can_decode = true;
        }
    }

    return 0;
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
    return pc;
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

static void (*decode(union instruction *fetched))(union instruction * ) {
    // TODO: Implement actual decoding
    return halt;

    switch (fetched->decoded.dp._id) {

    case DP_MULT_ID: {
        if (fetched->decoded.dp.imm_op == 1) {
            // return data processing
        }

        if (!fetched->decoded.mul._mul4) {
            // return data processing
        }

        if (!fetched->decoded.mul._mul7) {
            // return data processing
        }

        // return multiply
        break;
    }
    case SDT_ID: break; // return sdt

    case BRANCH_ID: break; // return branch

    default: assert(false); // Invalid instruction
    }

}

/* Dummy handler */
static void halt(union instruction *instr) {
    current = terminated;
}
