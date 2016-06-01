/* Module  : cond_map
 *
 * Usage   : Map string hash to ARM11 condition code
 *
 * Authors : Tencho Tenev
 */

#include "cond_map.h"

/* Pre-cmputed hashes for condition code strings */
#define _EQ 0x7165
#define _NE 0x656e
#define _GE 0x6567
#define _LT 0x746c
#define _GT 0x7467
#define _LE 0x656c

/* ARM11 condition codes */
#define EQ 0
#define NE 1
#define GE 0xa
#define LT 0xb
#define GT 0xc
#define LE 0xd
#define AL 0xe

uint32_t cond_map(uint32_t hash) {
    switch (hash) {
    case _EQ: return EQ;
    case _NE: return NE;
    case _GE: return GE;
    case _LT: return LT;
    case _GT: return GT;
    case _LE: return LE;
    default: return AL;
    }
}
