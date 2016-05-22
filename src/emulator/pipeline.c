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

#define WORD_SIZE 4

/* Pipeline state */

static enum status current = initial;

static uint32_t pc;

// static instruction *fetched, *decoded;

// static (*handler)(instruction*);

bool can_decode, can_execute;

/* End of Pipeline state*/

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
    // TODO
    return 1;

    if (current != initial) {
        return 1;
    }

    current = running;
    can_decode = can_execute = false;

    while (current == running) {
        if (can_decode) {
            if (can_execute) {
                // Execute
//                handler*(decoded);
            }

            // Decode
            can_execute = true;
            // decode will set can_execute to FALSE if this is a branch instruction
//            handler = decode(fetched);
//            decoded = fetched;
        }

        // Fetch
//        fetched = get_instruction(pc);
        can_decode = true;

        // Update PC
        pc += WORD_SIZE;
    }

    return 0;
}

/*
 * Function : reset
 * ----------------
 * Sets the status to initial. If emulate is running, it will return after
 * completing the current cycle.
 */
void reset() {
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
enum status get_status() {
    return current;
}

/*
 * Function : get_pc
 * Usage    : uint32_t pc = get_pc()
 * ---------------------------------
 * Provides read access to the program counter. Note that the program counter
 * is always exactly 8 bytes greater than the currently executed instruction
 */
uint32_t get_pc(void) {
    return pc;
}

/*
 * Function : decode
 * Usage    : handler = decode(fetched)
 * -----------------
 * Takes a pointer to an instruction and returns a function pointer to the
 * correct handler for the instruction type
 *
 * If the fetched instruction is branch also sets can_decode and can_execute to
 * FALSE
 */
//static void (*decode(*instruction fetched))(*instruction) {
// TODO
//}
