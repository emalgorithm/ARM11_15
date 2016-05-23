#ifndef BINUTILS_H
#define BINUTILS_H

#include <stdint.h>

uint32_t get_bits(uint32_t val, short pos, short count);

void set_byte_at(uint32_t *src, short pos, uint8_t byte);

uint32_t toggle_endian(uint32_t val);

#endif
