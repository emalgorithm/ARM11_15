#ifndef MULTIPLY_H
#define MULTIPLY_H

#include <stdio.h>
#include <stdint.h>
#include "arm11.h"
#include "util/cpsr_flags.h"

void mul_exec(union decoded_instr*);

#endif
