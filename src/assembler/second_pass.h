#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdint.h>

void sec_pass_run (const char *path);

uint32_t get_curr_instr_addr(void);

#endif
