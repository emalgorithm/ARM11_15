#include <assert.h>
#include <string.h>

#include "second_pass.h"
#include "../emulator/arm11.h"
#include "util/hashmap.h"
#include "util/func_hashmap.h"

func_map_t instr_map;
func_map_t dp_rd_map;
func_map_t dp_rn_map;
func_map_t dp_op2_map;

map_t opcode_map;
map_t shift_map;
map_t cond_map;

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

typedef void (*proc_pt)(char*, union decoded_instr*);

void proc_dp_instr (char*, union decoded_instr*);
void proc_mul_instr (char*, union decoded_instr*);
void proc_sdt_instr (char*, union decoded_instr*);
void proc_br_instr (char*, union decoded_instr*);
void proc_lsl_instr (char*, union decoded_instr*);

void dp_set_rd (char*, union decoded_instr*);
void dp_set_not_rd (char*, union decoded_instr*);
void dp_set_rn (char*, union decoded_instr*);
void dp_set_not_rn (char*, union decoded_instr*);

void dp_set_op2 (char*, union decoded_instr*);

void sec_pass_run (const char*);

void generate_maps () {
    instr_map = hashmap_new();
    dp_rd_map = hashmap_new();
    dp_rn_map = hashmap_new();

    opcode_map = hashmap_new();
    shift_map = hashmap_new();
    cond_map = hashmap_new();

    func_hashmap_put (instr_map, "add", proc_dp_instr);
    func_hashmap_put (instr_map, "sub", proc_dp_instr);
    func_hashmap_put (instr_map, "rsb", proc_dp_instr);
    func_hashmap_put (instr_map, "and", proc_dp_instr);
    func_hashmap_put (instr_map, "eor", proc_dp_instr);
    func_hashmap_put (instr_map, "orr", proc_dp_instr);
    func_hashmap_put (instr_map, "mov", proc_dp_instr);
    func_hashmap_put (instr_map, "tst", proc_dp_instr);
    func_hashmap_put (instr_map, "teq", proc_dp_instr);
    func_hashmap_put (instr_map, "cmp", proc_dp_instr);
    func_hashmap_put (instr_map, "mul", proc_mul_instr);
    func_hashmap_put (instr_map, "mla", proc_mul_instr);
    func_hashmap_put (instr_map, "ldr", proc_sdt_instr);
    func_hashmap_put (instr_map, "str", proc_sdt_instr);
    func_hashmap_put (instr_map, "b", proc_br_instr);
    func_hashmap_put (instr_map, "lsl", proc_lsl_instr);

    int ADD_OPCODE = 4;
    int SUB_OPCODE = 2;
    int RSB_OPCODE = 3;
    int AND_OPCODE = 0;
    int EOR_OPCODE = 1;
    int ORR_OPCODE = 12;
    int MOV_OPCODE = 13;
    int TST_OPCODE = 8;
    int TEQ_OPCODE = 9;
    int CMP_OPCODE = 10;

    hashmap_put (opcode_map, "add", (void *) &ADD_OPCODE);
    hashmap_put (opcode_map, "sub", (void *) &SUB_OPCODE);
    hashmap_put (opcode_map, "rsb", (void *) &RSB_OPCODE);
    hashmap_put (opcode_map, "and", (void *) &AND_OPCODE);
    hashmap_put (opcode_map, "eor", (void *) &EOR_OPCODE);
    hashmap_put (opcode_map, "orr", (void *) &ORR_OPCODE);
    hashmap_put (opcode_map, "mov", (void *) &MOV_OPCODE);
    hashmap_put (opcode_map, "tst", (void *) &TST_OPCODE);
    hashmap_put (opcode_map, "teq", (void *) &TEQ_OPCODE);
    hashmap_put (opcode_map, "cmp", (void *) &CMP_OPCODE);

    func_hashmap_put (dp_rd_map, "add", dp_set_rd);
    func_hashmap_put (dp_rd_map, "sub", dp_set_rd);
    func_hashmap_put (dp_rd_map, "rsb", dp_set_rd);
    func_hashmap_put (dp_rd_map, "and", dp_set_rd);
    func_hashmap_put (dp_rd_map, "eor", dp_set_rd);
    func_hashmap_put (dp_rd_map, "orr", dp_set_rd);
    func_hashmap_put (dp_rd_map, "mov", dp_set_rd);
    func_hashmap_put (dp_rd_map, "tst", dp_set_not_rd);
    func_hashmap_put (dp_rd_map, "teq", dp_set_not_rd);
    func_hashmap_put (dp_rd_map, "cmp", dp_set_not_rd);

    func_hashmap_put (dp_rn_map, "add", dp_set_rn);
    func_hashmap_put (dp_rn_map, "sub", dp_set_rn);
    func_hashmap_put (dp_rn_map, "rsb", dp_set_rn);
    func_hashmap_put (dp_rn_map, "and", dp_set_rn);
    func_hashmap_put (dp_rn_map, "eor", dp_set_rn);
    func_hashmap_put (dp_rn_map, "orr", dp_set_rn);
    func_hashmap_put (dp_rn_map, "mov", dp_set_not_rn);
    func_hashmap_put (dp_rn_map, "tst", dp_set_rn);
    func_hashmap_put (dp_rn_map, "teq", dp_set_rn);
    func_hashmap_put (dp_rn_map, "cmp", dp_set_rn);

    int LSL_VAL = 0;
    int LSR_VAL = 1;
    int ASR_VAL = 2;
    int ROR_VAL = 3;

    hashmap_put (shift_map, "lsl", (void *) &LSL_VAL);
    hashmap_put (shift_map, "lsr", (void *) &LSR_VAL);
    hashmap_put (shift_map, "asr", (void *) &ASR_VAL);
    hashmap_put (shift_map, "ror", (void *) &ROR_VAL);

    /*
    hashmap_put (cond_map, "eq", (void *) EQ);
    hashmap_put (cond_map, "ne", (void *) NE);
    hashmap_put (cond_map, "ge", (void *) GE);
    hashmap_put (cond_map, "lt", (void *) LT);
    hashmap_put (cond_map, "gt", (void *) GT);
    hashmap_put (cond_map, "le", (void *) LE);
    hashmap_put (cond_map, "al", (void *) AL); */


}

void proc_dp_instr(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    // Set Cond Field
    dp_instr->cond = 0xE;

    // Set Id Field
    dp_instr->cond = 0x0;

    // Set S Field
    //dp_instr->op_code = *((int *) hashmap_get(s_bit_map, dp_char));

    // Set OpCode
    dp_instr->op_code = *((int *) hashmap_get(opcode_map, dp_char));

    // Continue
    func_hashmap_get(dp_rd_map, dp_char)(dp_char, instruction);
}

void proc_mul_instr(char* mul_char, union decoded_instr* instruction) {

}

void proc_sdt_instr(char* sdt_char, union decoded_instr* instruction) {

}

void proc_br_instr(char* br_char, union decoded_instr* instruction) {

}

void proc_lsl_instr(char* lsl_char, union decoded_instr* instruction) {

}

// -----------------------------------------------------------------------

void dp_set_rd(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    // Set Rd register
    dp_instr->rd = tokreg();

    // Continue
    func_hashmap_get(dp_rn_map, dp_char)(dp_char, instruction);
}

void dp_set_not_rd(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    // No Rd Register
    dp_instr->rd = 0x0;

    // Continue
    dp_set_op2(dp_char, instruction);
}

void dp_set_rn(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    // Set Rn register
    dp_instr->rn = tokreg();

    // Continue
    func_hashmap_get(dp_op2_map, dp_char)(dp_char, instruction);
}

void dp_set_not_rn(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    // No Rn Register
    dp_instr->rn = 0x0;

    // Continue
    dp_set_op2(dp_char, instruction);
}

// ------------------------------------------------------------------

void dp_set_op2(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    union op2_gen* op2 = calloc(1, sizeof(union op2_gen));

    enum operandType* op_type = NULL;

    tokop(op_type);

    switch (*op_type) {

        case IMMEDIATE: ;
            // Union is of type op2_imm
            struct op2_imm* op2_imm = &op2->imm_op;
            // Set I Bit
            dp_instr->imm_op = 0x1;
            // Improper Call to tokimm. Must try rotation.
            op2_imm->imm = tokimm();
            op2_imm->rot = 0x0;
            break;
        case SHIFT_REG: ;
            // Union is of type op2_reg
            struct op2_reg* op2_reg = &op2->reg_op;
            // Not set I Bit
            dp_instr->imm_op = 0x0;
            // Set Rm
            op2_reg->rm = tokreg();

            enum operandType* sh_op_type = NULL;

            op2_reg->sh_ty = * ((int *)hashmap_get(shift_map, tokshift(sh_op_type)));

            switch (*sh_op_type) {
                case IMMEDIATE: ;
                    op2_reg->bit4 = 0;
                    op2_reg->shift_val = tokimm();
                    break;
                case SHIFT_REG: ;
                    op2_reg->bit4 = 1;
                    // Rotate left register to have empty bit7
                    op2_reg->shift_val = tokreg() << 1;
                    break;
            }
            break;
    }
    dp_instr->op2 = *((int *) op2);
}

// ------------------------------------------------------------------

void sec_pass_run (const char* path) {

    union decoded_instr* instruction;

    generate_maps();

    // Iinitialise tokeniser
    tokinit (path);

    while (hastok()) {
        instruction = calloc(1, sizeof(union decoded_instr));
        char* next = toknext();
        func_hashmap_get(instr_map, next)(next, instruction);

        // Free up memory

    }

    tokdestroy ();
}
