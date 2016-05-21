#ifndef SDT_H
#define SDT_H

#include <stdint.h>

void sdt_exect(void*);

struct sdt_instr {
	uint32_t cond: 4;
	uint32_t imm_offset: 1;
	uint32_t index_bit: 1;
	uint32_t up : 1;
	uint32_t load_store: 1;
	uint32_t base: 4;
	uint32_t src_dest: 4;
	uint32_t offset: 12;
};

#endif
