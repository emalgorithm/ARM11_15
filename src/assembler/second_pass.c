#include <assert.h>
#include <string.h>

#include "second_pass.h"
#include "../emulator/arm11.h"
#include "util/hashmap.h"
#include "util/func_hashmap.h"

func_map_t instr_map;
func_map_t opcode_map;
map_t cond_map;

typedef void (*proc_pt)(char*, union decoded_instr*);

void proc_dp_instr (char*, union decoded_instr*);
void proc_mul_instr (char*, union decoded_instr*);
void proc_sdt_instr (char*, union decoded_instr*);
void proc_br_instr (char*, union decoded_instr*);
void proc_lsl_instr (char*, union decoded_instr*);

void sec_pass_run (const char*);

void generate_maps () {
    instr_map = hashmap_new();
    opcode_map = hashmap_new();
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

    dp_instr->op_code = *((int *) hashmap_get(opcode_map, dp_char));
}

void proc_mul_instr(char* mul_char, union decoded_instr* instruction) {

}

void proc_sdt_instr(char* sdt_char, union decoded_instr* instruction) {

}

void proc_br_instr(char* br_char, union decoded_instr* instruction) {

}

void proc_lsl_instr(char* lsl_char, union decoded_instr* instruction) {

}

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
