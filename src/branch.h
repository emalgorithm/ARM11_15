#ifndef BRANCH_H
#define BRANCH

#include <stdint.h>

void br_exec(void*);

struct br_instr {
	uint32_t cond: 4;
	uint32_t offset: 24;
};

#endif 