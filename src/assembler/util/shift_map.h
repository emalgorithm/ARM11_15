/* Module  : shift_map
 *
 * Usage   : Map string hash to ARM11 shift code
 *
 * Authors : Tencho Tenev
 */

#ifndef SHIFTMAP_H
#define SHIFTMAP_H

#include <stdint.h>

uint32_t shift_map(uint32_t hash);

struct op2_imm {
    uint32_t imm : 8;
    uint32_t rot : 4;
};

struct op2_reg {
    uint32_t rm        : 4;
    uint32_t bit4      : 1;
    uint32_t sh_ty     : 2;
    uint32_t shift_val : 5;
};

union op2_gen {
    struct op2_reg reg_op;
    struct op2_imm imm_op;
};

#endif
