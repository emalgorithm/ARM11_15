#ifndef MULTIPLY_H
#define MULTIPLY_H

#include <stdint.h>

/* Include argument name in declarations? */
void mul_exec(void*);

struct mul_instr {
    uint32_t cond : 4;
    uint32_t acc: 1;
    uint32_t set_cond : 1;
    uint32_t dest : 4;
    uint32_t op1 : 4;
    uint32_t op2 : 4;
    uint32_t op3 : 4;
};

#endif
