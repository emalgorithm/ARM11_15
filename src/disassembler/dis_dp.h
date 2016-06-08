#ifndef DIS_DP
#define DIS_DP

#include <stdint.h>
#include "../emulator/arm11.h"

void dis_dp_instr(const char* path, union decoded_instr* instruction);

#endif
