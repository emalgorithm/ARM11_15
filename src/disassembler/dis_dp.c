#include <stdio.h>

#include "dis_dp.h"
#include "../assembler/util/func_hashmap.h"

map_t opcode_map;

void dis_generate_dp_maps () {

    opcode_map = func_hashmap_new();

    char* add_char = malloc(sizeof(uint32_t));
    char* mov_char = malloc(sizeof(uint32_t));

    sprintf(add_char, "%d", 4);
    sprintf(mov_char, "%d", 13);

    printf("%p\n", add_char);

    hashmap_put (opcode_map, add_char, (void *) "add");
    /*hashmap_put (opcode_map, 1, (void *) "sub");
    hashmap_put (opcode_map, 2, (void *) "rsb");
    hashmap_put (opcode_map, 3, (void *) "and");
    hashmap_put (opcode_map, 4, (void *) "eor");
    hashmap_put (opcode_map, 8, (void *) "orr"); */
    hashmap_put (opcode_map, mov_char, (void *) "mov");
    /*hashmap_put (opcode_map, 10, (void *) "tst");
    hashmap_put (opcode_map, 12, (void *) "teq");
    hashmap_put (opcode_map, 13, (void *) "cmp"); */
    //hashmap_put (opcode_map, ??, (void *) "lsl");

}

void dis_dp_instr(char* path, union decoded_instr* instruction) {

    //printf("\nGot Here\n");

    char* tmp_char = malloc(sizeof(uint32_t));

    sprintf(tmp_char, "%d", instruction->dp.op_code);

    printf("OpCode: %s\n", tmp_char);

    char* res_char = malloc(sizeof(uint32_t));

    res_char = (char *) hashmap_get(opcode_map, tmp_char);

    //printf("\nGot Here3\n");

    printf("OpCode: %s\n", res_char);

}
