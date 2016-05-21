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

    uint32_t result = ((*instr).acc) ? mul_accumulate() : mul_normal();

    arm11.registers[(*instr).dest] = result;
}

/* As in spec */
static uint32_t mul_accumulate(uint32_t m, uint32_t s, uint32_t n) {
    return m * s + n;
}

static uint32_t mul_normal(uint32_t m, uint32_t s) {
    return m * s;
}
