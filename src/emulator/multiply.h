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
    uint32_t rn : 4;
    uint32_t rs : 4;
    uint32_t rm : 4;
};

#endif
