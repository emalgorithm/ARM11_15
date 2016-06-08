/* Module  : pipeline
 *
 * Usage   : A top-level module which initialises arm11 can call emulate() to
 *           begin program execution. An instruction handler can read the
 *           program counter.
 *
 * Authors : Tencho Tenev
 */
#ifndef PIPELINE_H
#define PIPELINE_H

#include <stdint.h>

#define PC_OFFSET 8

/*
 * Enum : status
 * -------------
 * Represents the state of execution
 */
enum status {
    initial,
    running,
    terminated,
};

/* The 2 bits that come after Cond */
enum instr_id {
    DP_MULT_ID,
    SDT_ID,
    BRANCH_ID
};

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
int emulate(uint32_t pc_address);

/*
 * Function : em_reset
 * ----------------
 * Sets the status to initial. If emulate is running, it will return after
 * completing the current cycle.
 */
void em_reset();

/*
 * Function : em_get_status
 * Usage    : status em_status = em_get_status()
 * ---------------------------------
 * Returns one of initial, running, or terminated
 *      initial iff emulate has not been called
 *      running iff emulate was called and has not returned
 *      terminated iff emulate was called and returned
 */
enum status em_get_status();

/*
 * Function : em_get_pc
 * Usage    : uint32_t pc = em_get_pc()
 * ---------------------------------
 * Provides read access to the program counter. Note that the program counter
 * is always exactly 8 bytes greater than the currently executed instruction
 */
uint32_t em_get_pc(void);

/*
 * Function : em_set_pc
 * Usage    : set_pc(get_pc() + offset);
 * -------------------------------------
 * Set program counter (used when branching)
 */
void em_set_pc(uint32_t new_pc);

/*
 * Function : em_acc_pc
 * Usage    : em_acc_pc(offset);
 * -------------------------------------
 * Accumulate program counter (used when branching)
 */
void em_acc_pc(uint32_t offset);

#endif
