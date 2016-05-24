/* Module  : data_processing
 *
 * Usage   : A low-level module which processes instructions sent by the
 *           pipeline. Sets registers and CPSR flags accordingly.
 *
 * Authors : Alberto Spina
 */

#include <stdbool.h>
#include "data_processing.h"
#include "util/binutils.h"
#include "util/cpsr_flags.h"

/*Declarations*/

#define FOUR_BITS (0xF)
#define EIGHT_BITS (0xFF)
#define ROTATE_BIT (0x8)
#define MAX_BITS (0x20)
#define NFLAG_BIT (31)

/*Typedefs for Function Pointers*/

typedef void (*fun_ptr)(struct dp_instr * , bool);
typedef uint32_t (*shift_fun_ptr)(uint32_t, uint32_t);
typedef uint32_t (*op_exec_ptr)(uint32_t, uint32_t, uint32_t);

enum operators {AND, EOR, SUB, RSB, ORR};

void set_cflag_shift (uint32_t amount, uint32_t value, uint32_t index) {
    if (amount >= 1) {
        if (get_bit(value, index)) {
            set_cflag;
        } else {
            clr_cflag;
        }
    }
}

void set_cflag_cond (uint32_t set_cond, bool condition) {
    if (set_cond) {
        if (condition) {
            set_cflag;
        } else {
            clr_cflag;
        }
    }
}

/* */
uint32_t log_left_shift (uint32_t amount, uint32_t value) {
    set_cflag_shift(amount, value, MAX_BITS - amount);
    return value << amount;
}
uint32_t log_right_shift (uint32_t amount, uint32_t value) {
    set_cflag_shift(amount, value, amount);
    return (unsigned)value >> amount;
}
uint32_t arit_right_shift (uint32_t amount, uint32_t value) {
    set_cflag_shift(amount, value, MAX_BITS - amount);
    return value >> amount;
}
uint32_t rot_right (uint32_t amount, uint32_t value) {
    set_cflag_shift(amount, value, MAX_BITS - amount);
    uint32_t left_shift = (int)((unsigned)value >> amount);
    uint32_t right_shift = value << (MAX_BITS - amount);
    return left_shift | right_shift;
}

/**/
static uint32_t get_op2 (struct dp_instr* dp_instruction) {
    if (dp_instruction->imm_op) {
        /*Operand2 is an Immediate Operand*/
        uint32_t rotate = get_bits(dp_instruction->op2, 11, 4);
        uint32_t imm_val = get_bits(dp_instruction->op2, 7, 8);
        return rot_right(rotate, imm_val);
    } else {
        /*Operand2 is NOT an Immediate Operand*/
        uint32_t rm_reg = get_bits(dp_instruction->op2, 3, 4);
        uint32_t ctrl_bit = get_bits(dp_instruction->op2, 4, 1);
        uint32_t shift_type = get_bits(dp_instruction->op2, 6, 2);
        uint32_t rm_value = get_register(rm_reg);

        shift_fun_ptr shift_fun_ptr_array[] = {
            log_left_shift,
            log_right_shift,
            arit_right_shift,
            rot_right
        };

        if (ctrl_bit) {
            /*Shift is specified by a Register*/
            uint32_t rs_reg = get_bits(dp_instruction->op2, 11, 4);
            uint32_t rs_value = get_register(rs_reg);
            return shift_fun_ptr_array[shift_type](rs_value, rm_value);
        } else {
            /*Shift by a constant amount*/
            uint32_t integer = get_bits(dp_instruction->op2, 11, 5);
            return shift_fun_ptr_array[shift_type](integer, rm_value);
        }
    }
}

static void write_res (struct dp_instr* dp_instruction, bool write, uint32_t res) {
    if (write) {
        set_register(dp_instruction->rd, res);
    }
    if (dp_instruction->set_cond) {
        if (res == 0) {
            set_zflag;
        }
        if (get_bit(res, NFLAG_BIT)) {
            set_nflag;
        } else {
            clr_nflag;
        }
    }
}

static uint32_t and_op (uint32_t left, uint32_t right, uint32_t set_cond) {
    return left & right;
}

static uint32_t eor_op (uint32_t left, uint32_t right, uint32_t set_cond) {
    return left ^ right;
}

static uint32_t sub_op (uint32_t left, uint32_t right, uint32_t set_cond) {
    // Subraction produces a borrow if term 2 is less than term 1
    set_cflag_cond(set_cond, left < right);
    return left - right;
}

static uint32_t rsb_op (uint32_t left, uint32_t right, uint32_t set_cond) {
    // Subraction produces a borrow if term 2 is less than term 1
    set_cflag_cond(set_cond, right < left);
    return right - left;
}

static uint32_t orr_op (uint32_t left, uint32_t right, uint32_t set_cond) {
    return left | right;
}

static void exec_instr (struct dp_instr* dp_instruction, bool write, enum operators operator) {

    op_exec_ptr op_exec_ptr_array[] = {
        and_op,
        eor_op,
        sub_op,
        rsb_op,
        orr_op
    };

    uint32_t op1_val = get_register(dp_instruction->rn);
    uint32_t op2_val = get_op2(dp_instruction);
    uint32_t res = op_exec_ptr_array[operator](op1_val, op2_val, dp_instruction->set_cond);
    write_res(dp_instruction, write, res);
}

/**/
static void and_instr (struct dp_instr* dp_instruction, bool write) {
    exec_instr (dp_instruction, write, AND);
}

static void eor_instr (struct dp_instr* dp_instruction, bool write) {
    exec_instr (dp_instruction, write, EOR);
}

static void sub_instr (struct dp_instr* dp_instruction, bool write) {
    exec_instr (dp_instruction, write, SUB);
}

static void rsb_instr (struct dp_instr* dp_instruction, bool write) {
    exec_instr (dp_instruction, write, RSB);
}

static void add_instr (struct dp_instr* dp_instruction, bool write) {
    uint32_t op1_val = get_register(dp_instruction->rn);
    uint32_t op2_val = get_op2(dp_instruction);
    uint32_t res = op1_val + op2_val;
    write_res(dp_instruction, write, res);

    // Integer overflow = result is SMALLER than both addends
    set_cflag_cond(dp_instruction->set_cond, (res < op1_val) && (res < op2_val));
}

static void tst_instr (struct dp_instr* dp_instruction, bool write) {
    // Call AND with no write
    and_instr(dp_instruction, false);
}

static void teq_instr (struct dp_instr* dp_instruction, bool write) {
    // Call EOR with no write
    eor_instr(dp_instruction, false);
}

static void cmp_instr (struct dp_instr* dp_instruction, bool write) {
    // Call SUB with no write
    sub_instr(dp_instruction, false);
}

static void orr_instr (struct dp_instr* dp_instruction, bool write) {
    exec_instr (dp_instruction, write, ORR);
}

static void mov_instr (struct dp_instr* dp_instruction, bool write) {
    write_res(dp_instruction, write, get_op2(dp_instruction));
}

void dp_exec (union decoded_instr* instruction) {

    enum instr_op_code {AND, EOR, SUB, RSB, ADD, TST, TEQ, CMP, ORR, MOV};

	// Downcast void* to struct dp_instr* AND dereferencing.
    struct dp_instr* dp_instruction;
	dp_instruction = &instruction->dp;

    fun_ptr fun_ptr_array[] = {
        and_instr,
        eor_instr,
        sub_instr,
        rsb_instr,
        add_instr,
        tst_instr,
        teq_instr,
        cmp_instr,
        orr_instr,
        mov_instr
    };

    fun_ptr_array[dp_instruction->op_code](dp_instruction, true);
}
