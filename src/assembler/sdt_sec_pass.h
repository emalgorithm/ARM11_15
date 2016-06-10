/* Module  : sdt_sec_pass
 *
 * Usage   : Parse SDT instruction from tokeniser
 *
 * Authors : Tencho Tenev
 */

#ifndef SDT_SEC_PASS
#define SDT_SEC_PASS

#include "../emulator/arm11.h"

void proc_sdt_init();

void proc_sdt_instr(char *token, union decoded_instr *instr);

void write_data_section();

#endif
