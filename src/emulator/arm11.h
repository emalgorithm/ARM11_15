#ifndef ARM11_H
#define ARM11_H

#include <stdint.h>
#include <stdlib.h>

#define MEMORY_SIZE (1 << 16)
#define NUM_OF_REGISTERS 17

struct mul_instr {
    uint32_t cond : 4;
    uint32_t imm_op: 1;
    uint32_t set_cond : 1;
    uint32_t op1 : 4;
    uint32_t dest : 4;
    uint32_t op2 : 12;
};

struct br_instr {
	uint32_t cond: 4;
	uint32_t offset: 24;
};

struct dp_instr {
	uint32_t cond: 4;
	uint32_t imm_op: 1;
	uint32_t op_code: 4;
	uint32_t set_cond: 1;
	uint32_t op1: 4;
	uint32_t dest: 4;
	uint32_t op2: 12;
};

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

/* 
 * This procedure initialize memory and registers to 0
 */
void initialize(void);

/* 
 * This function takes an address in memory and 
 * returns the instruction starting at that address
 */
union instruction *get_instr(uint32_t address);

void set_register(uint32_t index, uint32_t value);

uint32_t get_register(uint32_t index);

void set_byte(uint32_t address, uint8_t value);

uint8_t get_byte(uint32_t address);

/* 
 * This procedure takes an address in memory and a 
 * 32-bit big endian value, and it writes the value 
 * at the desired address after converting it to
 * little endian
 */
void set_word(uint32_t address, uint32_t value);

/* 
 * This procedure takes an address in memory and 
 * it returns the word at the desired address 
 * after converting it to big endian
 */
uint32_t get_word(uint32_t address);

#endif
