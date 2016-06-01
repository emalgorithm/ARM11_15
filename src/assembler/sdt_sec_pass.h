#ifndef SDT_SEC_PASS
#define SDT_SEC_PASS

#include "writer_sec_pass.h"

void proc_sdt_instr (char*, union decoded_instr*);

void generate_sdt_maps ();

#endif
