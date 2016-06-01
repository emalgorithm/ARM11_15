#include "second_pass.h"

func_map_t instr_map;

typedef void (*proc_pt)(char*, union decoded_instr*);

void proc_lsl_instr (char*, union decoded_instr*);

void sec_pass_run (const char*);

void generate_maps () {
    instr_map = hashmap_new();

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
    func_hashmap_put (instr_map, "lsl", proc_dp_instr);
}

void proc_lsl_instr(char* lsl_char, union decoded_instr* instruction) {

}

void sec_pass_run (const char* path) {

    union decoded_instr* instruction;

    generate_maps();
    generate_dp_maps();
    generate_mul_maps();
    generate_br_maps();
    generate_sdt_maps();

    // Iinitialise tokeniser
    tokinit (path);

    char* next;

    while ((next = toknext()) != NULL) {
        instruction = calloc(1, sizeof(union decoded_instr));
        func_hashmap_get(instr_map, next)(next, instruction);
    }

    free(instruction);

    //tokdestroy ();
}
