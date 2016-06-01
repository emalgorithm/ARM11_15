#ifndef BR_SEC_PASS
#define BR_SEC_PASS

#include "../emulator/arm11.h"

void proc_br_instr (char*, union decoded_instr*);

void generate_br_maps ();

#endif
