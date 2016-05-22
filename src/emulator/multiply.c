/***************************************************************************
 * FILE NAME: multiply.c                                                   *
 *                                                                         *
 * PURPOSE: Carries out multiply instruction of ARM11                      *
 *          Accesses the ARM11 state to manipulate registers               *
 ***************************************************************************/

#include "multiply.h"

static uint32_t mul_accumulate();
static uint32_t mul_normal();

/* Pre: (*instruction).cond evaluates to true */
void mul_exec(void* instruction) {
    /* TODO: finish off skeleton of function */
    struct mul_instr* instr;
    instr = (struct mul_instr*) instruction;

    uint32_t result = ((*instr).imm_op) ? mul_accumulate() : mul_normal();

    set_register((*instr).dest, result);
}

/*
 * Function : mul_accumulate
 * -------------------------
 * Perform the arithmetic part of the multiply instruction.
 * Note: The result is casted to a 32bit integer (4 words). This function
 * should be sufficient for the specification of the instruction.
 */
static uint32_t mul_accumulate(uint32_t m, uint32_t s, uint32_t n) {
    return m * s + n;
}

static uint32_t mul_normal(uint32_t m, uint32_t s) {
    return m * s;
}
