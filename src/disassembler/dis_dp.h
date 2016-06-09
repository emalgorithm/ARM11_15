#ifndef DIS_DP
#define DIS_DP

#include <stdint.h>
#include "../emulator/arm11.h"
#include "../assembler/util/shift_map.h"

void dis_free_dp_maps ();

void dis_generate_dp_maps ();

void gen_op2(char*, union op2_gen*);

void dis_dp_instr(char* path, union decoded_instr* instruction);

#endif
