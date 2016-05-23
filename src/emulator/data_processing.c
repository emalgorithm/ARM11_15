/* Module  : data_processing
 *
 * Usage   : A low-level module which processes instructions sent by the
 *           pipeline. Sets registers and CPSR flags accordingly.
 *
 * Authors : Alberto Spina
 */

#include <stdbool.h>
#include "data_processing.h"
#include "arm11.h"
#include "util/binutils.h"

/*Declarations*/

int FOUR_BITS = 0xF;
int EIGHT_BITS = 0xFF;
int ROTATE_BIT = 8;
int MAX_BITS = 32;

/*Typedefs for Function Pointers*/

typedef void (*fun_ptr)(struct dp_instr, bool);
typedef uint32_t (*shift_fun_ptr)(uint32_t, uint32_t);

fun_ptr fun_ptr_array[9];
shift_fun_ptr shift_fun_ptr_array[3];

/* */
uint32_t log_left_shift (uint32_t amount, uint32_t value) {
    return value << amount;
}
uint32_t log_right_shift (uint32_t amount, uint32_t value) {
    return (unsigned)value >> amount;
}
uint32_t arit_right_shift (uint32_t amount, uint32_t value) {
    return value >> amount;
}
uint32_t rot_right (uint32_t amount, uint32_t value) {
    uint32_t left_shift = (int)((unsigned)value >> amount);
    uint32_t right_shift = value << (MAX_BITS - amount);
    return left_shift | right_shift;
}

/**/
static uint32_t get_op2 (struct dp_instr dp_instruction) {
    if (dp_instruction.imm_op) {
        /*Operand2 is an Immediate Operand*/
        uint32_t rotate = get_bits(dp_instruction.op2, 11, 4);
        uint32_t imm_val = get_bits(dp_instruction.op2, 7, 8);
        return rot_right(rotate, imm_val);
    } else {
        /*Operand2 is NOT an Immediate Operand*/
        uint32_t rm_reg = get_bits(dp_instruction.op2, 3, 4);
        uint32_t ctrl_bit = get_bits(dp_instruction.op2, 4, 1);
        uint32_t shift_type = get_bits(dp_instruction.op2, 6, 2);
        uint32_t rm_value = get_register(rm_reg);

        shift_fun_ptr_array[0] = log_left_shift;
        shift_fun_ptr_array[1] = log_right_shift;
        shift_fun_ptr_array[2] = arit_right_shift;
        shift_fun_ptr_array[3] = rot_right;

        if (ctrl_bit) {
            /*Shift is specified by a Register*/
            uint32_t rs_reg = get_bits(dp_instruction.op2, 11, 4);
            uint32_t rs_value = get_register(rs_reg);
            return shift_fun_ptr_array[shift_type](rs_value, rm_value);
        } else {
            /*Shift by a constant amount*/
            uint32_t integer = get_bits(dp_instruction.op2, 11, 5);
            return shift_fun_ptr_array[shift_type](integer, rm_value);
        }
    }
}

/**/
static void and_instr (struct dp_instr dp_instruction, bool write) {
    uint32_t op1_val = get_register(dp_instruction.op1);
    uint32_t res = op1_val & get_op2(dp_instruction);
    if (write) {
        set_register(dp_instruction.dest, res);
    }
}

static void eor_instr (struct dp_instr dp_instruction, bool write) {
    uint32_t op1_val = get_register(dp_instruction.op1);
    uint32_t res = op1_val ^ get_op2(dp_instruction);
    if (write) {
        set_register(dp_instruction.dest, res);
    }
}

static void sub_instr (struct dp_instr dp_instruction, bool write) {
    uint32_t op1_val = get_register(dp_instruction.op1);
    uint32_t res = op1_val - get_op2(dp_instruction);
    if (write) {
        set_register(dp_instruction.dest, res);
    }
}

static void rsb_instr (struct dp_instr dp_instruction, bool write) {
    uint32_t op1_val = get_register(dp_instruction.op1);
    uint32_t res = get_op2(dp_instruction) - op1_val;
    set_register(dp_instruction.dest, res);
}

static void add_instr (struct dp_instr dp_instruction, bool write) {
    uint32_t op1_val = get_register(dp_instruction.op1);
    uint32_t res = op1_val + get_op2(dp_instruction);
    set_register(dp_instruction.dest, res);
}

static void tst_instr (struct dp_instr dp_instruction, bool write) {
    // Call AND with no write
    and_instr(dp_instruction, false);
}

static void teq_instr (struct dp_instr dp_instruction, bool write) {
    // Call EOR with no write
    eor_instr(dp_instruction, false);
}

static void cmp_instr (struct dp_instr dp_instruction, bool write) {
    // Call SUB with no write
    sub_instr(dp_instruction, false);
}

static void orr_instr (struct dp_instr dp_instruction, bool write) {
    uint32_t op1_val = get_register(dp_instruction.op1);
    uint32_t res = op1_val | get_op2(dp_instruction);
    set_register(dp_instruction.dest, res);
}

static void mov_instr (struct dp_instr dp_instruction, bool write) {
    set_register(dp_instruction.dest, get_op2(dp_instruction));
}

void dp_exec (void* instruction) {

    enum instr_op_code {AND, EOR, SUB, RSB, ADD, TST, TEQ, CMP, ORR, MOV};

	// Downcast void* to struct dp_instr* AND dereferencing.
    struct dp_instr dp_instruction;
	dp_instruction = *((struct dp_instr *) instruction);

    fun_ptr_array[0] = and_instr;
    fun_ptr_array[1] = eor_instr;
    fun_ptr_array[2] = sub_instr;
    fun_ptr_array[3] = rsb_instr;
    fun_ptr_array[4] = add_instr;
    fun_ptr_array[5] = tst_instr;
    fun_ptr_array[6] = teq_instr;
    fun_ptr_array[7] = cmp_instr;
    fun_ptr_array[8] = orr_instr;
    fun_ptr_array[9] = mov_instr;

    fun_ptr_array[dp_instruction.op_code](dp_instruction, true);
}
