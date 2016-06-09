#include "branch.h"
#include <stdint.h>
#include <stdio.h>
#include "pipeline.h"
#include "../util/binutils.h"

#define OFFSET_SHIFT 2
#define OFFSET_SIZE 24

void br_exec(union decoded_instr *instruction) {
    int32_t offset = 0;

    compute_offset(instruction, &offset);

    em_acc_pc(offset);
}

void compute_offset(union decoded_instr *instruction, int32_t* offset) {
    *offset = instruction->br.offset;
    *offset = *offset << OFFSET_SHIFT;
    sign_extend(offset, OFFSET_SIZE - 1 + OFFSET_SHIFT);
}

void sign_extend(int32_t *val, short sign_pos) {
    uint32_t sign = get_bit(*val, sign_pos);

    if (sign) {
        *val |= (0xffffffff << sign_pos);
    }
}
