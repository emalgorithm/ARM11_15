/***************************************************************************
 * FILE NAME: multiply.c                                                   *
 *                                                                         *
 * PURPOSE: Carries out multiply instruction of ARM11                      *
 *          Accesses the ARM11 state to manipulate registers               *
 ***************************************************************************/

#include "multiply.h"

static uint32_t mul_accumulate(uint32_t, uint32_t, uint32_t);
static uint32_t mul_normal(uint32_t, uint32_t);

static void update_zflag(uint32_t);
static void update_nflag(uint32_t);

/* Pre: (*instruction).cond evaluates to true */
void mul_exec(union decoded_instr* instr) {
    /* Declare struct mul* instead? */
    uint32_t m = get_register((*instr).mul.rm);
    uint32_t n = get_register((*instr).mul.rn);
    uint32_t s = get_register((*instr).mul.rs);

    uint32_t result = ((*instr).mul.acc) ? mul_accumulate(m, s, n)
                                         : mul_normal(m, s);

    if ((*instr).mul.set_cond) {
        update_zflag(result);
        update_nflag(result);
    }

    set_register((*instr).mul.rd, result);
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
 * Procedures : update_zflag, update_nflag
 * ---------------------------------------
 * Handle instruction behaviour to do with setting the zero and negative flags.
 */
static void update_zflag(uint32_t val) {
    if (val == 0) {
        set_zflag;
    } else {
        clr_zflag;
    }
}

static void update_nflag(uint32_t val) {
    if (val & 1 << (WORD_SIZE * BYTE - 1)) {
        set_nflag;
    } else {
        clr_nflag;
    }
}
