#ifndef DIS_SDT
#define DIS_SDT

#include <stdint.h>
#include "../emulator/arm11.h"

void dis_sdt_instr(char* path, union decoded_instr* instruction);

#endif
