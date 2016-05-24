#include "branch.h"
#include <stdint.h>
#include "pipeline.h"
#include "util/binutils.h"

#define OFFSET_SHIFT 2
#define OFFSET_SIZE 24

static void sign_extend(int32_t *, short);

void br_exec(union decoded_instr *instruction) {
    int32_t offset = instruction->br.offset;

    offset = offset << OFFSET_SHIFT;
    sign_extend(&offset, OFFSET_SIZE - 1 + OFFSET_SHIFT);

    em_set_pc((int32_t)em_get_pc() + offset);
}

static void sign_extend(int32_t *val, short sign_pos) {
    uint32_t sign = get_bit(*val, sign_pos);

    if(sign) {
      *val |= (0xffffffff << sign_pos);
    }
}
