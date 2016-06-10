#include "str_util.h"

#include <string.h>
#include <assert.h>
#include <stdbool.h>

/* Copyright:
 * Adapted from: http://www.strudel.org.uk/itoa/
 */
char * itoa (int value, char *result, int base) {

    /**
	 * C++ version 0.4 char* style "itoa":
	 * Written by Lukás Chmela
	 * Released under GPLv3.

	 */

    // Cmake heck that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789ABCDEFghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }

    if ((result = realloc(result, strlen(result) + 1)) == NULL) {
        assert(false);
    }
    return result;
}

void concat(char *s1, char *s2) {
    // Would use realloc, but fails using valgrind

    char* res = calloc(strlen(s1)+strlen(s2)+1, sizeof(char));

    if (res == NULL) {
        assert(false);
    }
    strcat(res, s1);
    strcat(res, s2);
    strcpy(s1, res);

    free(res);
}

void gen_reg(char* empty_reg, uint32_t val){
    concat(empty_reg, "r");
    char* res = calloc(0, sizeof(char));
    res = itoa(val, res, 10);
    concat(empty_reg, res);
    free(res);
}

void gen_oxn(char* empty_reg, uint32_t val){
    concat(empty_reg, "#0x");
    char* res = calloc(0, sizeof(char));
    res = itoa(val, res, 16);
    concat(empty_reg, res);
    free(res);
}

void gen_int(char* empty_reg, uint32_t val){
    char* res = calloc(0, sizeof(char));
    res = itoa(val, res, 10);
    concat(empty_reg, res);
    free(res);
}
