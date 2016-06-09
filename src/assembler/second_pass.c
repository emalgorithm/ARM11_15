#include "second_pass.h"
#include "util/hashmap.h"
#include "util/func_hashmap.h"
#include "util/tokeniser.h"
#include "dp_sec_pass.h"
#include "br_sec_pass.h"
#include "sdt_sec_pass.h"
#include "mul_sec_pass.h"
#include "bwriter.h"

func_map_t instr_map;

typedef void (*proc_pt)(char*, union decoded_instr*);

void proc_lsl_instr (char*, union decoded_instr*);

void sec_pass_run (const char*);

static void halt(char* token, union decoded_instr* instruction) {
    return;
}

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
    func_hashmap_put (instr_map, "beq", proc_br_instr);
    func_hashmap_put (instr_map, "bne", proc_br_instr);
    func_hashmap_put (instr_map, "bge", proc_br_instr);
    func_hashmap_put (instr_map, "bgt", proc_br_instr);
    func_hashmap_put (instr_map, "ble", proc_br_instr);
    func_hashmap_put (instr_map, "bge", proc_br_instr);
    func_hashmap_put (instr_map, "lsl", proc_dp_instr);
    func_hashmap_put (instr_map, "andeq", halt);
}

static uint32_t curr_instr_addr;

uint32_t get_curr_instr_addr(void) {
    return curr_instr_addr;
}

void sec_pass_run (const char* path) {

    union decoded_instr* instruction;

    generate_maps();
    generate_dp_maps();
    proc_sdt_init();

    // Iinitialise tokeniser
    tokinit (path);


    curr_instr_addr = 0;

    char* next;

    while ((next = toknext()) != NULL) {
        instruction = calloc(1, sizeof(union decoded_instr));
        func_hashmap_get(instr_map, next)(next, instruction);
        bwr_instr(instruction);
        curr_instr_addr += 4;
        free(instruction);
    }


    write_data_section();

    //tokdestroy ();
}
