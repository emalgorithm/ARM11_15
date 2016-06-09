#ifndef DIS_BR
#define DIS_BR

#include <stdint.h>
#include "../emulator/arm11.h"

void dis_br_instr(char* path, union decoded_instr* instruction);

#endif
