#include <stdio.h>
#include <string.h>

#include "dis_mul.h"
#include "writer.h"
#include "util/str_util.h"

void dis_mul_instr(char* path, union decoded_instr* instruction) {

    //Assert Cond = 0xE and Assert S bit = 0

    char* rd = calloc(0, sizeof(char));
    char* rn = calloc(0, sizeof(char));
    char* rs = calloc(0, sizeof(char));
    char* rm = calloc(0, sizeof(char));
    char* res = calloc(0, sizeof(char));
    char* instr;

    concat(rd, " ");
    gen_reg(rd, instruction->mul.rd);
    concat(rm, ", ");
    gen_reg(rm, instruction->mul.rm);
    concat(rs, ", ");
    gen_reg(rs, instruction->mul.rs);

    if (instruction->mul.acc) {
        instr = "mla";
        concat(rn, ", ");
        gen_reg(rn, instruction->mul.rn);
    } else {
        instr = "mul";
        rn[0] = '\0';
    }

    concat(res, instr);
    concat(res, rd);
    concat(res, rm);
    concat(res, rs);
    concat(res, rn);
    concat(res, "\n");

    file_write(res);

    free(rd);
    free(rn);
    free(rs);
    free(rm);
    free(res);

}
