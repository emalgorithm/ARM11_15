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

int FOUR_BITS = 0xF;
int EIGHT_BITS = 0xFF;
int ROTATE_BIT = 8;
int MAX_BITS = 32;

static uint32_t get_op2 (struct dp_instr dp_instruction) {
    uint32_t rotate = (dp_instruction.op2  & ( FOUR_BITS << ROTATE_BIT )) >> ROTATE_BIT;
    uint32_t imm_val = dp_instruction.op2  & EIGHT_BITS;
    if (dp_instruction.imm_op) {
        uint32_t left_shift = (int)((unsigned)imm_val >> rotate);
        uint32_t right_shift = imm_val << (MAX_BITS - rotate);
        uint32_t new_op2 = left_shift | right_shift;
        return new_op2;
    } else {
        return 0;
    }
}

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
    and_instr(dp_instruction, false);
}

static void teq_instr (struct dp_instr dp_instruction, bool write) {
    eor_instr(dp_instruction, false);
}

static void cmp_instr (struct dp_instr dp_instruction, bool write) {
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

    typedef void (*fun_ptr)(struct dp_instr, bool);
	
	/*Work In Progress*/

	/*Declarations*/
	
	struct dp_instr dp_instruction;
	fun_ptr fun_ptr_array[9];    
    enum instr_op_code {AND, EOR, SUB, RSB, ADD, TST, TEQ, CMP, ORR, MOV};

	// Downcast void* to struct dp_instr* AND dereferencing.	
	dp_instruction = *((struct dp_instr *) instruction);
	
	//Instruction fields	
	//uint32_t imm_op = dp_instruction.imm_op;
	//uint32_t op_code = dp_instruction.op_code;
    //uint32_t set_cond = dp_instruction.set_cond;
    //uint32_t op1 = dp_instruction.op1;
    //uint32_t dest = dp_instruction.dest;
    //uint32_t op2 = dp_instruction.op2;
    
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
	
	//uint32_t res = (uint32_t) -1 + (uint32_t) 2;
	//printf("%d %x %o\n", 10, (uint32_t) res, 10);
	
	/* OLD SWITCH CODE
	switch (op_code) {
	    case AND:	        
	        res = op1_val & op2;
	        set_register(dest, res);
	        break;
        case EOR:
            res = op1_val ^ op2;
            set_register(dest, res);
            break;
        case SUB:
            res = op1_val - op2;
            set_register(dest, res);
            break;
        case RSB:
            res = op2 - op1_val;
            set_register(dest, res);
            break;
        case ADD:
            res = op2 + op1_val;
            set_register(dest, res);
            break;
        case TST:
            // Loop?
            // FUnction with AND behaviour? Applies also below
            res = 0;
            break;
        case TEQ:
            res = 0;
            break;
        case CMP:
            res = 0;
            break;
        case ORR:
            res = op1_val | op2;
            set_register(dest, res);
            break;
        case MOV:
            res = op2;
            set_register(dest, res);
            break;
        default:
            printf("INVALID OPCODE!\n");
            break;
	} */
	
	// Logical right shift	
	//uint32_t bob = (unsigned)-1 >> 1;
	//printf("%d %x\n", bob, (uint32_t) bob);
	
	// Arithmetic right shift
	//uint32_t bob = -1 >> 1;
	//printf("%d %x\n", bob, (uint32_t) bob);
	
	//Testing:
	
	// Debugging
	/*printf("AND, OpCode: %d\n", op_code);	        
	        printf("Op1_val: %d\n", op1_val);
	        printf("Op2: %d\n", op2);
	        printf("Res: %d\n", res); */
	
}



