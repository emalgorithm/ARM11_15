#ifndef ARM11_H
#define ARM11_H

#include <stdint.h>

#define MEMORY_SIZE (1 << 16)
#define NUM_OF_REGISTERS 17

struct state {
    uint8_t memory[MEMORY_SIZE];  
    uint32_t registers[NUM_OF_REGISTERS];
};

extern struct state arm11;

void initialize(void);

#endif
