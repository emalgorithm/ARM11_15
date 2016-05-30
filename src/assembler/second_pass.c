#include <assert.h>
#include <string.h>

#include "second_pass.h"
#include "../emulator/arm11.h"
#include "util/hashmap.h"

map_t instr_map;
map_t cond_map;

enum instr_enum {
    ADD,
    SUB,
    RSB,
    AND,
    EOR,
    ORR,
    MOVE,
    TST,
    TEQ,
    CMP,
    MUL,
    MLA,
    LDR,
    STR,
    B,
    LSL
};

enum cond_enum {
    EQ,
    NE,
    GE,
    LT,
    GT,
    LE,
    AL
};

typedef void (*proc_pt)(enum instr_enum);

void proc_dp_instr (enum instr_enum);
void proc_mul_instr (enum instr_enum);
void proc_sdt_instr (enum instr_enum);
void proc_br_instr (enum instr_enum);
void proc_lsl_instr (enum instr_enum);

void sec_pass_run (const char*);

proc_pt proc_arr[16];

void generate_maps () {
    instr_map = hashmap_new();
    cond_map = hashmap_new();

    hashmap_put (instr_map, "add", (void *) ADD);
    hashmap_put (instr_map, "sub", (void *) SUB);
    hashmap_put (instr_map, "rsb", (void *) RSB);
    hashmap_put (instr_map, "and", (void *) AND);
    hashmap_put (instr_map, "eor", (void *) EOR);
    hashmap_put (instr_map, "orr", (void *) ORR);
    hashmap_put (instr_map, "mov", (void *) MOV);
    hashmap_put (instr_map, "tst", (void *) TST);
    hashmap_put (instr_map, "teq", (void *) TEQ);
    hashmap_put (instr_map, "cmp", (void *) CMP);
    hashmap_put (instr_map, "mul", (void *) MUL);
    hashmap_put (instr_map, "mla", (void *) MLA);
    hashmap_put (instr_map, "ldr", (void *) LDR);
    hashmap_put (instr_map, "str", (void *) STR);
    hashmap_put (instr_map, "b", (void *) B);
    hashmap_put (instr_map, "lsl", (void *) LSL);

    hashmap_put (cond_map, "eq", (void *) EQ);
    hashmap_put (cond_map, "ne", (void *) NE);
    hashmap_put (cond_map, "ge", (void *) GE);
    hashmap_put (cond_map, "lt", (void *) LT);
    hashmap_put (cond_map, "gt", (void *) GT);
    hashmap_put (cond_map, "le", (void *) LE);
    hashmap_put (cond_map, "al", (void *) AL);
}

void generate_arrays () {
    proc_pt temp_proc_arr[] = {
        proc_dp_instr, //ADD
        proc_dp_instr, //SUB
        proc_dp_instr, //RSB
        proc_dp_instr, //AND
        proc_dp_instr, //EOR
        proc_dp_instr, //ORR
        proc_dp_instr, //MOV
        proc_dp_instr, //TST
        proc_dp_instr, //TEQ
        proc_dp_instr, //CMP
        proc_mul_instr, //MUL
        proc_mul_instr, //MLA
        proc_sdt_instr, //LDR
        proc_sdt_instr, //STR
        proc_br_instr, //B
        proc_lsl_instr, //LSL
    };

    memcpy(proc_arr, temp_proc_arr, sizeof proc_arr);
}

void proc_dp_instr(enum instr_enum dp_enum) {

}

void proc_mul_instr(enum instr_enum mul_enum) {

}

void proc_sdt_instr(enum instr_enum sdt_enum) {

}

void proc_br_instr(enum instr_enum br_enum) {

}

void proc_lsl_instr(enum instr_enum lsl_enum) {

}

void sec_pass_run (const char* path) {

    //union decoded_instr instruction;

    generate_maps();
    generate_arrays();

    // Iinitialise tokeniser
    tokinit (path);

    while (hastok()) {
        char* next = toknext();
        enum instr_enum instr_enum = (enum instr_enum) hashmap_get(instr_map, next);

        proc_arr[instr_enum](instr_enum);

    }

    tokdestroy ();
}
