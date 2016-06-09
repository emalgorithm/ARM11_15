#include <stdio.h>
#include <string.h>

#include "dis_dp.h"
#include "writer.h"
#include "../assembler/util/func_hashmap.h"
#include "../emulator/util/shift_reg.h"

map_t opcode_map;

func_map_t dis_dp_rd_map;
func_map_t dis_dp_rn_map;

void dis_dp_set_rd (char*, union decoded_instr*);
void dis_dp_set_not_rd (char*, union decoded_instr*);
void dis_dp_set_rn (char*, union decoded_instr*);
void dis_dp_set_rn_first (char*, union decoded_instr*);
void dis_dp_set_not_rn (char*, union decoded_instr*);

char * itoa (int value, char *result, int base);
char* concat(char *s1, char *s2);

void gen_op2(char*, union op2_gen*);

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
    func_hashmap_put (dis_dp_rn_map, "tst", dis_dp_set_rn_first);
    func_hashmap_put (dis_dp_rn_map, "teq", dis_dp_set_rn_first);
    func_hashmap_put (dis_dp_rn_map, "cmp", dis_dp_set_rn_first);
    //func_hashmap_put (dp_rn_map, "lsl", dp_lsl);

}

void dis_dp_instr(char* path, union decoded_instr* instruction) {
    //ADD ASSERTION FOR CONDITION

    //ADD ASSERTION FOR SET CONDITIONS

    char* rn = malloc(sizeof(char));
    char* rd = malloc(sizeof(char));
    char* op2 = malloc(sizeof(char));
    char* tmp = malloc(sizeof(char));
    char* res = malloc(sizeof(char));
    char* tmp_char = malloc(sizeof(char));

    char* instr;

    itoa(instruction->dp.op_code, tmp, 10);
    tmp_char = concat(tmp_char, tmp);

    tmp[0] = '\0';

    instr = (char *) hashmap_get(opcode_map, tmp_char);

    func_hashmap_get(dis_dp_rd_map, instr)(rn, instruction);
    func_hashmap_get(dis_dp_rn_map, instr)(rd, instruction);

    // Create op2_gen struct and initialise it to the binary
    union op2_gen* op2_gen = malloc(sizeof(union op2_gen));
    op2_gen->bin = instruction->dp.op2;

    uint32_t operand2;

    if (instruction->dp.imm_op) {
        operand2 = rot_right(op2_gen->imm_op.rot*2, op2_gen->imm_op.imm, 0);

        op2 = concat(op2, "#0x");
        itoa(operand2, tmp, 16);
        op2 = concat(op2, tmp);
        tmp[0] = '\0';

    } else {
        gen_op2(op2, op2_gen);
    }

    //Write Statement



    res = concat(res, instr);
    res = concat(res, rn);
    res = concat(res, rd);
    res = concat(res, ", ");
    res = concat(res, op2);
    res = concat(res, "\n");

    file_write(res);

    free(tmp);
    free(tmp_char);
    free(res);
    free(rn);
    free(rd);
    free(op2);

}

void gen_op2(char* op2, union op2_gen* op2_gen) {
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

        sprintf(op2 , "r%d, %s r%d", op2_gen->reg_op.rm, shift, op2_gen->reg_op.shift_val>>1);

    } else {
        sprintf(op2 , "r%d, #0x%X", op2_gen->reg_op.rm, op2_gen->reg_op.shift_val);
    }
}

void dis_dp_set_rd(char* dest, union decoded_instr* instruction) {

    dest = concat(dest, " r");

    char* res = malloc(sizeof(char));
    itoa(instruction->dp.rd, res, 10);
    dest = concat(dest, res);
    free(res);

}

void dis_dp_set_not_rd(char* dest, union decoded_instr* instruction) {

    dest[0] = '\0';

}

void dis_dp_set_rn(char* dest, union decoded_instr* instruction) {

    concat(dest, ",");

    dis_dp_set_rn_first(dest, instruction);

}

void dis_dp_set_rn_first(char* dest, union decoded_instr* instruction) {

    dest = concat(dest, " r");
    char* res = malloc(sizeof(char));
    itoa(instruction->dp.rn, res, 10);
    dest = concat(dest, res);
    free(res);
}

void dis_dp_set_not_rn(char* dest, union decoded_instr* instruction) {

    dest[0] = '\0';

}

char * itoa (int value, char *result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);

    return result;
}
