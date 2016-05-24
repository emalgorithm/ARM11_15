/***************************************************************************
 * FILE NAME: multiply.c                                                   *
 *                                                                         *
 * PURPOSE: Carries out multiply instruction of ARM11                      *
 *          Accesses the ARM11 state to manipulate registers               *
 ***************************************************************************/

#include "multiply.h"

static uint32_t mul_accumulate(uint32_t, uint32_t, uint32_t);
static uint32_t mul_normal(uint32_t, uint32_t);

static void update_cpsr_flag(uint32_t result);
static uint32_t check_zero(uint32_t);
static uint32_t check_neg(uint32_t);

/* Pre: (*instruction).cond evaluates to true */
void mul_exec(union decoded_instr* instr) {
    /* Declare struct mul* instead? */
    uint32_t m = get_register((*instr).mul.rm);
    uint32_t n = get_register((*instr).mul.rn);
    uint32_t s = get_register((*instr).mul.rs);

    uint32_t result = ((*instr).mul.acc) ? mul_accumulate(m, s, n)
                                         : mul_normal(m, s);

    if ((*instr).mul.set_cond) {
        update_cpsr_flag(result);
    }

    set_register((*instr).mul.rd, result);
}

/*
 * Procedure : update_cpsr_flag
 * ----------------------------
 * Handle instruction behaviour to do with setting flags.
 */
static void update_cpsr_flag(uint32_t result) {
    uint32_t cpsr_change = check_zero(result) | check_neg(result);
    set_register(CPSR_REG, cpsr_change);
}

/*
 * Function : mul_accumulate
 * -------------------------
 * Perform the arithmetic part of the multiply instruction, accumulate variant.
 * Note: The result is casted to a 32bit integer (4 words). This function
 * should be sufficient for the specification of the instruction.
 */
static uint32_t mul_accumulate(uint32_t m, uint32_t s, uint32_t n) {
    uint32_t ms = mul_normal(m, s);
    return ms + n;
}

/*
 * Function : mul_normal
 * ---------------------
 * Perform the multiply instruction, non-accumulate variant.
 * Same note applies as for mul_accumulate.
 */
static uint32_t mul_normal(uint32_t m, uint32_t s) {
    return m * s;
}

/*
 * Function : check_zero
 * ---------------------
 * Satisfies the update conditions for the flag Z.
 * Returns the flag <-> result of multiply is zero.
 */
static uint32_t check_zero(uint32_t val) {
    return (val) ? 0 : CPSR_Z;
}

/*
 * Function : check_neg
 * --------------------
 * Satisfies the update conditions for the flag N.
 * Returns the flag <-> result of multiply is zero.
 */
static uint32_t check_neg(uint32_t val) {
    return (val & SIGN_BIT) ? CPSR_N : 0;
}
