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
map_t s_bit_map;
map_t cond_map;

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

void sec_pass_run (const char*);

void generate_maps () {
    instr_map = hashmap_new();
    dp_rd_map = hashmap_new();
    dp_rn_map = hashmap_new();

    opcode_map = hashmap_new();
    s_bit_map = hashmap_new();
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

    int ADD_S_BIT = 0;
    int SUB_S_BIT = 0;
    int RSB_S_BIT = 0;
    int AND_S_BIT = 0;
    int EOR_S_BIT = 0;
    int ORR_S_BIT = 0;
    int MOV_S_BIT = 0;
    int TST_S_BIT = 1;
    int TEQ_S_BIT = 1;
    int CMP_S_BIT = 1;

    hashmap_put (s_bit_map, "add", (void *) &ADD_S_BIT);
    hashmap_put (s_bit_map, "sub", (void *) &SUB_S_BIT);
    hashmap_put (s_bit_map, "rsb", (void *) &RSB_S_BIT);
    hashmap_put (s_bit_map, "and", (void *) &AND_S_BIT);
    hashmap_put (s_bit_map, "eor", (void *) &EOR_S_BIT);
    hashmap_put (s_bit_map, "orr", (void *) &ORR_S_BIT);
    hashmap_put (s_bit_map, "mov", (void *) &MOV_S_BIT);
    hashmap_put (s_bit_map, "tst", (void *) &TST_S_BIT);
    hashmap_put (s_bit_map, "teq", (void *) &TEQ_S_BIT);
    hashmap_put (s_bit_map, "cmp", (void *) &CMP_S_BIT);

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
    dp_instr->op_code = *((int *) hashmap_get(s_bit_map, dp_char));

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
    func_hashmap_get(dp_rn_map, dp_char)(dp_char, instruction);
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
    func_hashmap_get(dp_op2_map, dp_char)(dp_char, instruction);
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
