/* Module  : hash_string.c
 *
 * Usage   : Primitive string hashing which uses the first 4 characters
 *
 * Authors : Tencho Tenev
 */

#include "hash_string.h"

#define STRLEN 4

uint32_t hash(const char *str) {
    uint32_t hash = 0;

    for (short count = 0; *str != '\0' && count < STRLEN; count++) {
        hash |= (*str++) << (8 * count);
    }

    return hash;
}
