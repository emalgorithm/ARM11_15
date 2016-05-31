#include <assert.h>
#include <string.h>

#include "second_pass.h"
#include "../emulator/arm11.h"
#include "util/hashmap.h"
//#include "util/func_hashmap.h"

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

    func_hashmap_put (instr_map, "add", (void *) proc_dp_instr);
    func_hashmap_put (instr_map, "sub", (void *) proc_dp_instr);
    func_hashmap_put (instr_map, "rsb", (void *) proc_dp_instr);
    func_hashmap_put (instr_map, "and", (void *) proc_dp_instr);
    func_hashmap_put (instr_map, "eor", (void *) proc_dp_instr);
    func_hashmap_put (instr_map, "orr", (void *) proc_dp_instr);
    func_hashmap_put (instr_map, "mov", (void *) proc_dp_instr);
    func_hashmap_put (instr_map, "tst", (void *) proc_dp_instr);
    func_hashmap_put (instr_map, "teq", (void *) proc_dp_instr);
    func_hashmap_put (instr_map, "cmp", (void *) proc_dp_instr);
    func_hashmap_put (instr_map, "mul", (void *) proc_mul_instr);
    func_hashmap_put (instr_map, "mla", (void *) proc_mul_instr);
    func_hashmap_put (instr_map, "ldr", (void *) proc_sdt_instr);
    func_hashmap_put (instr_map, "str", (void *) proc_sdt_instr);
    func_hashmap_put (instr_map, "b", (void *) proc_br_instr);
    func_hashmap_put (instr_map, "lsl", (void *) proc_lsl_instr);

    hashmap_put (opcode_map, "add", (void *) &(4));
    hashmap_put (opcode_map, "sub", (void *) &2);
    hashmap_put (opcode_map, "rsb", (void *) &3);
    hashmap_put (opcode_map, "and", (void *) &0);
    hashmap_put (opcode_map, "eor", (void *) &1);
    hashmap_put (opcode_map, "orr", (void *) &12);
    hashmap_put (opcode_map, "mov", (void *) &13);
    hashmap_put (opcode_map, "tst", (void *) &8);
    hashmap_put (opcode_map, "teq", (void *) &9);
    hashmap_put (opcode_map, "cmp", (void *) &10);

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
    generate_arrays();

    // Iinitialise tokeniser
    tokinit (path);

    while (hastok()) {
        instruction = calloc(1, sizeof(union decoded_instr));
        char* next = toknext();
        func_hashmap_get(instr_map, next)(next, union);

        // Free up memory

    }

    tokdestroy ();
}
