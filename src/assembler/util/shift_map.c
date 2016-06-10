/* Module  : shift_map
 *
 * Usage   : Map string hash to ARM11 shift code
 *
 * Authors : Tencho Tenev
 */

#include "shift_map.h"

/* Pre-cmputed hashes for shift code strings */
#define _LSL 0x6c736c
#define _LSR 0x72736c
#define _ASR 0x727361
#define _ROR 0x726f72

/* ARM11 shift codes */
#define LSL 0
#define LSR 1
#define ASR 2
#define ROR 3

uint32_t shift_map(uint32_t hash) {
    switch (hash) {
    case _LSL: return LSL;
    case _LSR: return LSR;
    case _ASR: return ASR;
    case _ROR: return ROR;
    default: return LSL;
    }
}
