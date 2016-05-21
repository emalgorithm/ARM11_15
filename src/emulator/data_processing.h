#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

#include <stdio.h>

void dp_exec(void*);

struct dp_instr {
	uint32_t cond: 4;
	uint32_t imm_op: 1;
	uint32_t op_code: 4;
	uint32_t set_cond: 1;
	uint32_t op1: 4;
	uint32_t dest: 4;
	uint32_t op2: 12;
};

#endif /*DATA_PROCESSING_H*/
