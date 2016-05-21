#include "data_processing.h"


void dp_exec (void* instruction) {
	
	/*Work In Progress*/

	/*OpCode enum*/
	typedef enum {AND, EOR, SUB, RSB, ADD, TST, TEQ, CMP, ORR, MOV} op_code;
	
	/*Declarations*/
	typedef struct dp_instr dp_instr_struct;

	/*Put Value at instruction in dp_instr*/
	dp_instr_struct = *(struct dp_instr)instruction;

	/*Switch here for the two cases of Operand 2*/

	/*Define the behaviour according to the given OpCode*/

	switch (dp_instr_struct.op_code) {
		case AND:
			break;
		case EOR:
			break;
		case SUB:
			break;
		case RSB:
			break;
		case ADD:
			break;
		case TST:
			break;
		case TEQ:
			break;
		case CMP:
			break;
		case ORR:
			break;
		case MOV:
			break;
	}


}

