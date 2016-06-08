#include <stdio.h>
#include <string.h>

#include "dis_mul.h"
#include "writer.h"

void dis_mul_instr(char* path, union decoded_instr* instruction) {

    //Assert Cond = 0xE and Assert S bit = 0

    char* rd = malloc(sizeof(char));
    char* rn = malloc(sizeof(char));
    char* rs = malloc(sizeof(char));
    char* rm = malloc(sizeof(char));
    char* instr;

    sprintf(rd , " r%d", instruction->mul.rd);
    sprintf(rn , ", r%d", instruction->mul.rn);
    sprintf(rs , ", r%d", instruction->mul.rs);

    if (instruction->mul.acc) {
        instr = "mla";
        sprintf(rm , ", r%d", instruction->mul.rm);
    } else {
        instr = "mul";
        rm[0] = '\0';
    }

    char* res = malloc(strlen(rd) + strlen(rn) + strlen(rs) + strlen(rm)+1);

    sprintf(res, "%s%s%s%s%s\n", instr, rd, rn, rs, rm);

    file_write(res);

}
