#include "arm11.h"
#include <assert.h>
#include <stdlib.h>

#define BYTE 8
#define LSB 0x000000ff
#define ASSERT_ADDRESS(address) assert(address >= 0 && address < MEMORY_SIZE)
#define ASSERT_INDEX(index) assert(index >= 0 && index < NUM_OF_REGISTERS)


struct state {
    uint8_t *memory;  
    uint32_t *registers;
};

struct word {
	uint32_t lsb: 8;
	uint32_t b2: 8;
	uint32_t b3: 8;
	uint32_t msb: 8;
};

union decoded_instr {
	struct mul_instr mul;
	struct br_instr br;
	struct dp_instr dp;
	struct sdt_instr sdt;
};

union instruction {
	struct word raw;
	union decoded_instr decoded;
};

static struct state arm11;

/* 
 * This procedure initialize memory and registers to 0
 */
void initialize(void) {
	arm11.memory = (uint8_t *)calloc(MEMORY_SIZE, sizeof(uint8_t));
	arm11.registers = (uint32_t *)calloc(NUM_OF_REGISTERS, sizeof(uint32_t));
}

/* 
 * This function takes an address in memory and 
 * returns the instruction starting at that address
 */
union instruction *get_instr(uint32_t address) {
	struct word raw;


	raw.lsb = arm11.memory[address++];
	raw.b2 = arm11.memory[address++];
	raw.b3 = arm11.memory[address++];
	raw.msb = arm11.memory[address];

	union instruction *instr = malloc(sizeof *instr);

	instr->
	raw = raw;

	return instr;
}

void set_register(uint32_t index, uint32_t value) {
	ASSERT_INDEX(index);

	arm11.registers[index] = value;
}

uint32_t get_register(uint32_t index) {
	ASSERT_INDEX(index);
 
	return arm11.registers[index];
}

void set_byte(uint32_t address, uint8_t value) {
	ASSERT_ADDRESS(address);

	arm11.memory[address] = value;
}

uint8_t get_byte(uint32_t address) {
	ASSERT_ADDRESS(address);

	return arm11.memory[address];
}

/* 
 * This procedure takes an address in memory and a 
 * 32-bit big endian value, and it writes the value 
 * at the desired address after converting it to
 * little endian
 */
void set_word(uint32_t address, uint32_t value) {
	ASSERT_ADDRESS(address);

	uint32_t mask = LSB;

	arm11.memory[address++] = (uint8_t)(value & mask);
	value = value >> BYTE;
	arm11.memory[address++] = (uint8_t)(value & mask);
	value = value >> BYTE;
	arm11.memory[address++] = (uint8_t)(value & mask);
	value = value >> BYTE;
	arm11.memory[address] = (uint8_t)(value & mask);
}

/* 
 * This procedure takes an address in memory and 
 * it returns the word (32-bit) starting at the 
 * desired address after converting it to big endian
 */
uint32_t get_word(uint32_t address){
	ASSERT_ADDRESS(address);

    uint32_t word = ((uint32_t)arm11.memory[address++]);
    word |= ((uint32_t)arm11.memory[address++]) << BYTE;
    word |= ((uint32_t)arm11.memory[address++]) << 2 * BYTE;
    word |= ((uint32_t)arm11.memory[address]) << 3 * BYTE;

    return word;
}

