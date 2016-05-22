/***************************************************************************
 * FILE NAME: pipeline.c                                                   *
 *                                                                         *
 * PURPOSE: Emulate a three-stage pipeline as in ARM 11.                   *
 *                                                                         *
 * DEVELOPMENT HISTORY:                                                    *
 *                                                                         *
 * Date          Author                Description of Changes              *
 * ----          ------                ----------------------              *
 * 20/05/2016    Tencho Tenev          Initial version                     *
 ***************************************************************************/

#include "./util/binutils.h"
#include <assert.h>

/* Global state */
extern struct state arm11;

/* Pipeline state */

// /* 4 bytes from arm11 memory */
// static uint32_t fetched;

// /* 4 bytes from arm 11 memory which are prepared for execution */
// static uint32_t decoded;

// /* A pointer for handling the decoded instruction */
// static (*handler)(void*);
// static void *instruction;

// static uint32_t pc;

/* End of Pipeline state*/


/* Utility functions */

// static void init_pipeline(void);
// static uint32_t getWordAt(uint32_t address);

/* End of utility functions */

/* PRE:  arm11 memory is initialised and populated with the executable binary
 * POST:
 *       if emulate returns 0
 *           state of arm11 registers corresponds to the termination state of
 *           the binary
 *
 *       otherwise
 *            the return value corresponds to an error code
 */
int emulate(void) {
    get_bits(15, 3, 2);
    //init_pipeline();

    // A 0 instruction terminates
    // while (!decoded) {
    //     // Execute decoded instruction
    //     //*handler(decoded);

    //     // Decode fetched
    //     //decode(fetched);

    //     // Fetch next
    //     //pc += INSTRUCTION_SIZE;
    //     //fetched = getWordAt(pc);
    // }

    return 0;
}

/* */
// void decode(uint32_t ) {

// }
