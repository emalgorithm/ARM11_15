#include "dis_sdt.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../assembler/util/shift_map.h"
#include "../emulator/util/shift_reg.h"
#include "../emulator/reader.h"
#include "../emulator/arm11.h"
#include "dis_dp.h"
#include "dis_exec.h"
#include "writer.h"
#include "util/str_util.h"

#define PROC_COUNT (15)

static int num_loads;

void dis_sdt_instr(char* path, union decoded_instr* instruction) {

    char* op2 = calloc(0, sizeof(char));
    char* rn = calloc(0, sizeof(char));
    char* rd = calloc(0, sizeof(char));
    char* plus_mins = calloc(0, sizeof(char));
    char* clsr_br1 = calloc(0, sizeof(char));
    char* opn_br = calloc(0, sizeof(char));
    char* clsr_br2 = calloc(0, sizeof(char));
    char* res = calloc(0, sizeof(char));

    char* instr;

    num_loads = 0;

    opn_br[0] = '\0';
    clsr_br1[0] = '\0';
    clsr_br2[0] = '\0';
    plus_mins[0] = '\0';

    // Create op2_gen struct and initialise it to the binary
    union op2_gen* op2_gen = malloc(sizeof(union op2_gen));
    op2_gen->bin = instruction->sdt.offset;

    union instruction* full_instr;

    uint32_t operand2;

    if(instruction->sdt.imm_off) {
        gen_op2(op2, op2_gen);
    } else {
        operand2 = rot_right(op2_gen->imm_op.rot*2, op2_gen->imm_op.imm, 0);
        concat(op2, ", ");
        gen_oxn(op2, operand2);
    }

    if(instruction->sdt.rn==PROC_COUNT){
        op2[0] = '\0';
        full_instr = get_instr(get_max_pc() + num_loads*4);
        operand2 = full_instr->bin;

        concat(op2, "=");
        gen_oxn(op2, operand2);

        num_loads ++;

    } else {
        if(instruction->sdt.index_bit) {
            clsr_br2 = "]";
        } else {
            clsr_br1 = "]";
        }
        opn_br = "[";

        gen_reg(rn, instruction->sdt.rn);
    }
    gen_reg(rd, instruction->sdt.rd);

    if(instruction->sdt.load_store) {
        instr = "ldr";
    } else {
        instr = "str";
    }

    if(!instruction->sdt.up){
        plus_mins = "-";
    }

    concat(res, instr);
    concat(res, " ");
    concat(res, rd);
    concat(res, ", ");
    concat(res, opn_br);
    concat(res, rn);
    concat(res, clsr_br1);
    concat(res, plus_mins);
    concat(res, op2);
    concat(res, clsr_br2);
    concat(res, "\n");

    file_write(res);

    free(op2);
    free(rn);
    free(rd);
    //TODO: The following?
    //free(plus_mins);
    //free(clsr_br1);
    //free(clsr_br2);
    //free(opn_br);
    free(res);

}
