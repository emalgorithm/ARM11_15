#include "data_processing.h"
#include "arm11.h"


void dp_exec (void* instruction) {
	
	/*Work In Progress*/

	/*Declarations*/
    
    enum instr_op_code {AND, EOR, SUB, RSB, ADD, TST, TEQ, CMP, ORR, MOV};

	/*Put Value at instruction in dp_instr*/
	
	struct dp_instr dp_instruction = *((struct dp_instr *) instruction);

	/*Switch here for the two cases of Operand 2*/
	
	/*Switch over the op_code*/
	
    //uint32_t imm_op = dp_instruction.imm_op;
	uint32_t op_code = dp_instruction.op_code;
    //uint32_t set_cond = dp_instruction.set_cond;
    uint32_t op1 = dp_instruction.op1;
    uint32_t dest = dp_instruction.dest;
    uint32_t op2 = dp_instruction.op2;
    
    uint32_t op1_val = get_register(op1);
    uint32_t res;
	
	//uint32_t res = (uint32_t) -1 + (uint32_t) 2;
	//printf("%d %x %o\n", 10, (uint32_t) res, 10);
	
	switch (op_code) {
	    case AND:	        
	        res = op1_val & op2;
	        /*
	        printf("AND, OpCode: %d\n", op_code);	        
	        printf("Op1_val: %d\n", op1_val);
	        printf("Op2: %d\n", op2);
	        printf("Res: %d\n", res); */
	        set_register(dest, res);
	        break;
        case EOR:
            res = op1_val ^ op2;
            //printf("EOR, OpCode: %d\n", op_code);
            set_register(dest, res);
            break;
        case SUB:
            printf("SUB, OpCode: %d\n", op_code);
            break;
        case RSB:
            printf("RSB, OpCode: %d\n", op_code);
            break;
        case ADD:
            printf("ADD, OpCode: %d\n", op_code);
            break;
        case TST:
            printf("TST, OpCode: %d\n", op_code);
            break;
        case TEQ:
            printf("TEQ, OpCode: %d\n", op_code);
            break;
        case CMP:
            printf("CMP, OpCode: %d\n", op_code);
            break;
        case ORR:
            printf("ORR, OpCode: %d\n", op_code);
            break;
        case MOV:
            printf("MOV, OpCode: %d\n", op_code);
            break;
        default:
            printf("INVALID OPCODE!\n");
            break;
	}

}



