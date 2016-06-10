#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>
#include "arm11.h"

void br_exec(union decoded_instr *);

void compute_offset(union decoded_instr*, int32_t*);

void sign_extend(int32_t* , short);

#endif
