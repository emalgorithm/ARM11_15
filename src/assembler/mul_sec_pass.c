#include "mul_sec_pass.h"
#include "util/tokeniser.h"
#include <string.h>

#define MUL_COND 0xE

void proc_mul_instr(char *mul_char, union decoded_instr *decoded) {
    uint8_t rd = tokreg();
    uint8_t rm = tokreg();
    uint8_t rs = tokreg();

    decoded->mul.cond = MUL_COND;
    decoded->mul.rd = rd;
    decoded->mul.rm = rm;
    decoded->mul.rs = rs;

    if (strcmp(mul_char, "mla") == 0) {
        decoded->mul.acc = 1;
        uint8_t rn = tokreg();
        decoded->mul.rn = rn;
    }
}
