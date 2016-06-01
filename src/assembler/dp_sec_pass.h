#ifndef DP_SEC_PASS
#define DP_SEC_PASS

#include "../emulator/arm11.h"

void proc_dp_instr (char*, union decoded_instr*);

void generate_dp_maps ();

#endif
