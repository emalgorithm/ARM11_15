#include "data_processing.h"
#include "arm11.h"


void dp_exec (void* instruction) {
	
	/*Work In Progress*/

	/*OpCode enum*/
	
	uint32_t AND = 0;
	uint32_t EOR = 1;
	uint32_t SUB = 2;
	uint32_t RSB = 3;
	uint32_t ADD = 4;
	uint32_t TST = 5;
	uint32_t TEQ = 6;
	uint32_t CMP = 7;
	uint32_t ORR = 8;
	uint32_t MOV = 9;

	/*Declarations*/
	//struct dp_instr dp_instruction;

	/*Put Value at instruction in dp_instr*/
	struct dp_instr dp_instruction;

	dp_instruction = *((struct dp_instr *) instruction);

	printf("Pointer: %p", (void *) &dp_instruction);

	/*Switch here for the two cases of Operand 2*/

	/*Define the behaviour according to the given OpCode*/

	uint32_t op_code = dp_instruction.op_code;

	if (op_code == AND) {
		printf("AND, OpCode: %d", AND);
	} 
	else if (op_code == EOR) {
		printf("EOR, OpCode: %d", EOR);
	}
	else if (op_code == SUB) {
		printf("SUB, OpCode: %d", SUB);
	}
	else if (op_code == RSB) {
		printf("RSB, OpCode: %d", RSB);
	}
	else if (op_code == ADD) {
		printf("ADD, OpCode: %d", ADD);
	}
	else if (op_code == TST) {
		printf("TST, OpCode: %d", TST);
	}
	else if (op_code == TEQ) {
		printf("TEQ, OpCode: %d", TEQ);
	}
	else if (op_code == CMP) {
		printf("CMP, OpCode: %d", CMP);
	}
	else if (op_code == ORR) {
		printf("ORR, OpCode: %d", ORR);
	}
	else if (op_code == MOV) {
		printf("MOV, OpCode: %d", MOV);
	}
	else {
		printf("Invalid OpCode!");
	}
	
	


}

