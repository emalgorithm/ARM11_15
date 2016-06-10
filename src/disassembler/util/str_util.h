#ifndef STR_UTIL
#define STR_UTIL

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char * itoa (int value, char *result, int base);
void concat(char *s1, char *s2);
void gen_reg(char* empty_reg, uint32_t val);
void gen_oxn(char* empty_reg, uint32_t val);
void gen_int(char* empty_reg, uint32_t val);

#endif
