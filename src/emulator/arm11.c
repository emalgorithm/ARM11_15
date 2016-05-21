#include "arm11.h"

struct state {
    uint8_t *memory;  
    uint32_t *registers;
};

struct word {
	uint32_t b1: 8;
	uint32_t b2: 8;
	uint32_t b3: 8;
	uint32_t b4: 8;
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
