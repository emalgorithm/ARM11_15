#include <stdio.h>

#include "dis_dp.h"
#include "../assembler/util/func_hashmap.h"
#include "../assembler/util/shift_map.h"
#include "../emulator/util/shift_reg.h"

map_t opcode_map;

func_map_t dis_dp_rd_map;
func_map_t dis_dp_rn_map;

void dis_dp_set_rd (char*, union decoded_instr*);
void dis_dp_set_not_rd (char*, union decoded_instr*);
void dis_dp_set_rn (char*, union decoded_instr*);
void dis_dp_set_not_rn (char*, union decoded_instr*);

void dis_generate_dp_maps () {

    opcode_map = hashmap_new();

    dis_dp_rd_map = func_hashmap_new();
    dis_dp_rn_map = func_hashmap_new();

    char* add_char = malloc(sizeof(uint32_t));
    char* sub_char = malloc(sizeof(uint32_t));
    char* rsb_char = malloc(sizeof(uint32_t));
    char* and_char = malloc(sizeof(uint32_t));
    char* eor_char = malloc(sizeof(uint32_t));
    char* orr_char = malloc(sizeof(uint32_t));
    char* mov_char = malloc(sizeof(uint32_t));
    char* tst_char = malloc(sizeof(uint32_t));
    char* teq_char = malloc(sizeof(uint32_t));
    char* cmp_char = malloc(sizeof(uint32_t));

    sprintf(add_char, "%d", 4);
    sprintf(sub_char, "%d", 2);
    sprintf(rsb_char, "%d", 3);
    sprintf(and_char, "%d", 0);
    sprintf(eor_char, "%d", 1);
    sprintf(orr_char, "%d", 12);
    sprintf(mov_char, "%d", 13);
    sprintf(tst_char, "%d", 8);
    sprintf(teq_char, "%d", 9);
    sprintf(cmp_char, "%d", 10);

    printf("%p\n", add_char);

    hashmap_put (opcode_map, add_char, (void *) "add");
    hashmap_put (opcode_map, sub_char, (void *) "sub");
    hashmap_put (opcode_map, rsb_char, (void *) "rsb");
    hashmap_put (opcode_map, and_char, (void *) "and");
    hashmap_put (opcode_map, eor_char, (void *) "eor");
    hashmap_put (opcode_map, orr_char, (void *) "orr");
    hashmap_put (opcode_map, mov_char, (void *) "mov");
    hashmap_put (opcode_map, tst_char, (void *) "tst");
    hashmap_put (opcode_map, teq_char, (void *) "teq");
    hashmap_put (opcode_map, cmp_char, (void *) "cmp");
    //hashmap_put (opcode_map, ??, (void *) "lsl");

    func_hashmap_put (dis_dp_rd_map, "add", dis_dp_set_rd);
    func_hashmap_put (dis_dp_rd_map, "sub", dis_dp_set_rd);
    func_hashmap_put (dis_dp_rd_map, "rsb", dis_dp_set_rd);
    func_hashmap_put (dis_dp_rd_map, "and", dis_dp_set_rd);
    func_hashmap_put (dis_dp_rd_map, "eor", dis_dp_set_rd);
    func_hashmap_put (dis_dp_rd_map, "orr", dis_dp_set_rd);
    func_hashmap_put (dis_dp_rd_map, "mov", dis_dp_set_rd);
    func_hashmap_put (dis_dp_rd_map, "tst", dis_dp_set_not_rd);
    func_hashmap_put (dis_dp_rd_map, "teq", dis_dp_set_not_rd);
    func_hashmap_put (dis_dp_rd_map, "cmp", dis_dp_set_not_rd);
    //func_hashmap_put (dp_rd_map, "lsl", dp_set_rd);

    func_hashmap_put (dis_dp_rn_map, "add", dis_dp_set_rn);
    func_hashmap_put (dis_dp_rn_map, "sub", dis_dp_set_rn);
    func_hashmap_put (dis_dp_rn_map, "rsb", dis_dp_set_rn);
    func_hashmap_put (dis_dp_rn_map, "and", dis_dp_set_rn);
    func_hashmap_put (dis_dp_rn_map, "eor", dis_dp_set_rn);
    func_hashmap_put (dis_dp_rn_map, "orr", dis_dp_set_rn);
    func_hashmap_put (dis_dp_rn_map, "mov", dis_dp_set_not_rn);
    func_hashmap_put (dis_dp_rn_map, "tst", dis_dp_set_rn);
    func_hashmap_put (dis_dp_rn_map, "teq", dis_dp_set_rn);
    func_hashmap_put (dis_dp_rn_map, "cmp", dis_dp_set_rn);
    //func_hashmap_put (dp_rn_map, "lsl", dp_lsl);

}

void dis_dp_instr(char* path, union decoded_instr* instruction) {

    //printf("\nGot Here\n");

    char* tmp_char = malloc(sizeof(uint32_t));

    sprintf(tmp_char, "%d", instruction->dp.op_code);

    //printf("OpCode: %s\n", tmp_char);

    char* instr = malloc(sizeof(uint32_t));

    instr = (char *) hashmap_get(opcode_map, tmp_char);

    //printf("\nGot Here3\n");

    //printf("OpCode: %s\n", instr);

    //ADD ASSERTION FOR CONDITION

    //ADD ASSERTION FOR SET CONDITIONS

    char* rn = malloc(sizeof(uint32_t));
    char* rd = malloc(sizeof(uint32_t));
    char* op2 = malloc(sizeof(uint32_t));

    func_hashmap_get(dis_dp_rd_map, instr)(rn, instruction);
    func_hashmap_get(dis_dp_rn_map, instr)(rd, instruction);

    // Create op2_gen struct and initialise it to the binary
    union op2_gen* op2_gen = malloc(sizeof(union op2_gen));
    op2_gen->bin = instruction->dp.op2;

    uint32_t operand2;

    if (instruction->dp.imm_op) {
        operand2 = rot_right(op2_gen->imm_op.rot, op2_gen->imm_op.imm, 0);
        sprintf(op2 , ", #%d", operand2);
    } else {
        if (op2_gen->reg_op.bit4) {
            char* shift = '\0';
            switch (op2_gen->reg_op.sh_ty) {
                case 0:
                    shift = "lsl";
                    break;
                case 1:
                    shift = "lsr";
                    break;
                case 2:
                    shift = "asr";
                    break;
                case 3:
                    shift = "ror";
                    break;
            }
            sprintf(op2 , ", r%d, %s r%d", op2_gen->reg_op.rm, shift, op2_gen->reg_op.shift_val>>1);

        } else {
            sprintf(op2 , ", r%d, #%d", op2_gen->reg_op.rm, op2_gen->reg_op.shift_val);
        }
    }

    //Write Statement
    printf("%s%s%s%s\n", instr, rn, rd, op2);


}

void dis_dp_set_rd (char*, union decoded_instr*);
void dis_dp_set_not_rd (char*, union decoded_instr*);
void dis_dp_set_rn (char*, union decoded_instr*);
void dis_dp_set_not_rn (char*, union decoded_instr*);

void dis_dp_set_rd(char* dest, union decoded_instr* instruction) {

    sprintf(dest , " r%d", instruction->dp.rd);

}

void dis_dp_set_not_rd(char* dest, union decoded_instr* instruction) {

    dest[0] = '\0';

}

void dis_dp_set_rn(char* dest, union decoded_instr* instruction) {

    sprintf(dest , ", r%d", instruction->dp.rn);

}

void dis_dp_set_not_rn(char* dest, union decoded_instr* instruction) {

    dest[0] = '\0';

}
