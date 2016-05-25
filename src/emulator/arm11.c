#include "arm11.h"
#include "util/binutils.h"
#include <assert.h>
#include <stdlib.h>

#define LSB 0x000000ff
#define WORD_ADDRESS(address) address / WORD_SIZE
#define ASSERT_ADDRESS(address) assert(address >= 0 && address < MEMORY_SIZE)
#define ASSERT_WORD_ADDRESS_ALIGNED(address) assert(address >= 0 && address < MEMORY_SIZE && address % WORD_SIZE == 0)
#define ASSERT_INDEX(index) assert(index >= 0 && index < NUM_OF_REGISTERS)



struct state {
    union instruction *memory;
    uint32_t *registers;
};

static struct state arm11;

/*
 * This procedure initialize memory and registers to 0
 */
void initialize(void) {
    arm11.memory = (union instruction *)calloc(MEMORY_SIZE, sizeof(uint8_t));
    arm11.registers = (uint32_t *)calloc(NUM_OF_REGISTERS, sizeof(uint32_t));
}

/*
 * This function takes an address in memory and
 * returns the instruction starting at that address
 */
union instruction *get_instr(uint32_t address) {
    ASSERT_ADDRESS(address);

    return &(arm11.memory[WORD_ADDRESS(address)]);
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

    uint32_t offset = (address % WORD_SIZE) * BYTE;

    uint32_t clear_mask = ~(0xff << offset);

    arm11.memory[WORD_ADDRESS(address)].bin = (arm11.memory[WORD_ADDRESS(address)].bin & clear_mask)
            | (((uint32_t)value) << offset);
}

uint8_t get_byte(uint32_t address) {
    ASSERT_ADDRESS(address);

    uint32_t offset = (address % WORD_SIZE) * BYTE;

    return get_bits(arm11.memory[WORD_ADDRESS(address)].bin, offset + BYTE - 1, BYTE);
}

/*
 * This procedure takes an address in memory and a
 * 32-bit big endian value, and it writes the value
 * at the desired address after converting it to
 * little endian
 */
void set_word(uint32_t address, uint32_t value) {
    ASSERT_ADDRESS(address);

    set_byte(address, get_bits(value, BYTE - 1, BYTE));
    set_byte(address + 1, get_bits(value, 2 * BYTE - 1, BYTE));
    set_byte(address + 2, get_bits(value, 3 * BYTE - 1, BYTE));
    set_byte(address + 3, get_bits(value, 4 * BYTE - 1, BYTE));
}

/*
 * This procedure takes an aligned address in memory and a
 * 32-bit big endian value, and it writes the value
 * at the desired address after converting it to
 * little endian
 */
void set_word_aligned(uint32_t address, uint32_t value) {
    ASSERT_WORD_ADDRESS_ALIGNED(address);

    arm11.memory[WORD_ADDRESS(address)].bin = value;
}

/*
 * This procedure takes an address in memory and
 * it returns the word (32-bit) starting at the
 * desired address after converting it to big endian
 */
uint32_t get_word(uint32_t address) {
    ASSERT_ADDRESS(address);

    uint32_t word  = get_byte(address);
    word |= (get_byte(address + 1) << BYTE);
    word |= (get_byte(address + 2) << 2 * BYTE);
    word |= (get_byte(address + 3) << 3 * BYTE);

    return word;
}

/*
 * This procedure takes an aligned address in memory and
 * it returns the word (32-bit) starting at the
 * desired address after converting it to big endian
 */
uint32_t get_word_aligned(uint32_t address) {
    ASSERT_WORD_ADDRESS_ALIGNED(address);

    return arm11.memory[WORD_ADDRESS(address)].bin;
}
