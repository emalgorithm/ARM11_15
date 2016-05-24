
/***************************************************************************
 * FILE NAME: shift_reg.h                                                  *
 *                                                                         *
 * PURPOSE: Helper functions for shifting                                  *
 *                                                                         *
 * DEVELOPMENT HISTORY:                                                    *
 *                                                                         *
 * Date          Author                Description of Changes              *
 * ----          ------                ----------------------              *
 * 24/05/2016    Alberto Spina         Initial version                     *
 ***************************************************************************/

#include "shift_reg.h"
#include "binutils.h"
#include "cpsr_flags.h"

/*Declarations*/

#define FOUR_BITS (0xF)
#define EIGHT_BITS (0xFF)
#define ROTATE_BIT (0x8)
#define MAX_BITS (0x20)
#define NFLAG_BIT (31)

typedef uint32_t (*shift_fun_ptr)(uint32_t, uint32_t, uint32_t);

static void set_cflag_shift (uint32_t amount, uint32_t value, uint32_t index, uint32_t set_cond) {
    if (amount >= 1 && set_cond) {
        if (get_bit(value, index)) {
            set_cflag;
        } else {
            clr_cflag;
        }
    }
}

/* */
static uint32_t log_left_shift (uint32_t amount, uint32_t value, uint32_t set_cond) {
    set_cflag_shift(amount, value, MAX_BITS - amount, set_cond);
    return value << amount;
}
static uint32_t log_right_shift (uint32_t amount, uint32_t value, uint32_t set_cond) {
    set_cflag_shift(amount, value, amount, set_cond);
    return (unsigned)value >> amount;
}
static uint32_t arit_right_shift (uint32_t amount, uint32_t value, uint32_t set_cond) {
    set_cflag_shift(amount, value, MAX_BITS - amount, set_cond);
    return value >> amount;
}
 uint32_t rot_right (uint32_t amount, uint32_t value, uint32_t set_cond) {
    set_cflag_shift(amount, value, MAX_BITS - amount, set_cond);
    uint32_t left_shift = (int)((unsigned)value >> amount);
    uint32_t right_shift = value << (MAX_BITS - amount);
    return left_shift | right_shift;
}

uint32_t shift_reg (uint32_t instruction, uint32_t set_cond) {

    uint32_t rm_reg = get_bits(instruction, 3, 4);
    uint32_t ctrl_bit = get_bits(instruction, 4, 1);
    uint32_t shift_type = get_bits(instruction, 6, 2);
    uint32_t rm_value = get_register(rm_reg);

    shift_fun_ptr shift_fun_ptr_array[] = {
        log_left_shift,
        log_right_shift,
        arit_right_shift,
        rot_right
    };

    if (ctrl_bit) {
        /*Shift is specified by a Register*/
        uint32_t rs_reg = get_bits(instruction, 11, 4);
        uint32_t rs_value = get_register(rs_reg);
        return shift_fun_ptr_array[shift_type](rs_value, rm_value, set_cond);
    } else {
        /*Shift by a constant amount*/
        uint32_t integer = get_bits(instruction, 11, 5);
        return shift_fun_ptr_array[shift_type](integer, rm_value, set_cond);
    }


}
