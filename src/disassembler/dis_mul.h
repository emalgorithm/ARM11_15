#ifndef DIS_MUL
#define DIS_MUL

#include <stdint.h>
#include "../emulator/arm11.h"

void dis_mul_instr(const char* path, union decoded_instr* instruction);

#endif
