#include "data_processing.h"
#include "arm11.h"


void dp_exec (void* instruction) {
	
	/*Work In Progress*/

	/*Declarations*/
    
    enum instr_op_code {AND, EOR, SUB, RSB, ADD, TST, TEQ, CMP, ORR, MOV};

	/*Put Value at instruction in dp_instr*/
	
	struct dp_instr dp_instruction = *((struct dp_instr *) instruction);

	/*Switch here for the two cases of Operand 2*/


	/*Different behaviours acco*/

	uint32_t op_code = dp_instruction.op_code;
	
	switch (op_code) {
	    case AND:
	        printf("AND, OpCode: %d/n", op_code);
	        break;
        case EOR:
            printf("EOR, OpCode: %d/n", op_code);
            break;
        case SUB:
            printf("SUB, OpCode: %d/n", op_code);
            break;
        case RSB:
            printf("RSB, OpCode: %d/n", op_code);
            break;
        case ADD:
            printf("ADD, OpCode: %d/n", op_code);
            break;
        case TST:
            printf("TST, OpCode: %d/n", op_code);
            break;
        case TEQ:
            printf("TEQ, OpCode: %d/n", op_code);
            break;
        case CMP:
            printf("CMP, OpCode: %d/n", op_code);
            break;
        case ORR:
            printf("ORR, OpCode: %d/n", op_code);
            break;
        case MOV:
            printf("MOV, OpCode: %d/n", op_code);
            break;
        default:
            printf("INVALID OPCODE!/n");
            break;
	}

}



