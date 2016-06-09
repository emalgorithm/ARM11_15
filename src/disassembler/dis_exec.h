#ifndef DIS_EXEC
#define DIS_EXEC

#include <stdint.h>

void disassemble_run (char *path);

void inc_pc();

uint32_t get_pc();

uint32_t get_max_pc();

void set_max_pc();

#endif
