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

#define PROC_COUNT (15)

static int num_loads;

void dis_sdt_instr(char* path, union decoded_instr* instruction) {

    char* op2 = malloc(sizeof(char));
    char* clsr_br1 = malloc(sizeof(char));
    char* opn_br = malloc(sizeof(char));
    char* clsr_br2 = malloc(sizeof(char));
    char* instr = malloc(sizeof(char));
    char* rn = malloc(sizeof(char));
    char* rd = malloc(sizeof(char));
    char* plus_mins = malloc(sizeof(char));

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
        sprintf(op2 , ", #0x%X", operand2);
    }

    if(instruction->sdt.rn==PROC_COUNT){
        op2[0] = '\0';
        full_instr = get_instr(get_max_pc() + num_loads*4);
        operand2 = full_instr->bin;

        sprintf(op2 , "=#0x%X", operand2);

        num_loads ++;

    } else {
        if(instruction->sdt.index_bit) {
            clsr_br2[0] = ']';
        } else {
            clsr_br1[0] = ']';
        }
        opn_br[0] = '[';

        sprintf(rn, "r%d", instruction->sdt.rn);
    }
    sprintf(rd, "r%d", instruction->sdt.rd);

    if(instruction->sdt.load_store) {
        instr = "ldr";
    } else {
        instr = "str";
    }

    if(!instruction->sdt.up){
        plus_mins[0] = '-';
    }

    char* res = malloc(sizeof(char));

    sprintf(res, "%s %s, %s%s%s%s%s%s\n", instr, rd, opn_br, rn, clsr_br1, plus_mins, op2, clsr_br2);

    file_write(res);

}
