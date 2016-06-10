/* Module  : hash_string.c
 *
 * Usage   : Primitive string hashing which uses the first 4 characters
 *
 * Authors : Tencho Tenev
 */

#ifndef HASHSTRING_H
#define HASHSTRING_H

#include <stdint.h>

uint32_t hash(const char *str);

#endif
