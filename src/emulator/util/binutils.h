#ifndef BINUTILS_H
#define BINUTILS_H

#include <stdint.h>

uint32_t getbits(uint32_t val, short pos, short count);

uint32_t toggle_endian(uint32_t val);

uint32_t get_word(uint8_t memory[], uint32_t address);

#endif
