
#include "../assembler/util/hashmap.h"
#include "../emulator/branch.h"
#include "../emulator/reader.h"
#include "../emulator/arm11.h"
#include "dis_label.h"
#include "dis_exec.h"
#include "writer.h"
#include "util/str_util.h"

#include <string.h>
#include <assert.h>

#define OFFSET_SHIFT 2
#define OFFSET_SIZE 24

static map_t dis_label_map;

static uint32_t num_labels;

void dis_scan_init() {
    dis_label_map = hashmap_new();
    num_labels = 0;

    bool running = true;

    union instruction* instruction;

    uint32_t pc = 0;

    while(running) {
        instruction = get_instr(pc);

        if (instruction->decoded.br._id == 2) {

            int32_t offset = 0;

            char* tmp_char = calloc(0, sizeof(char));
            char* label_char = calloc(0, sizeof(char));


            compute_offset(&instruction->decoded, &offset);

            gen_int(tmp_char, (pc + offset + 8));

            concat(label_char, "label_");
            gen_int(label_char, num_labels);

            hashmap_put (dis_label_map, tmp_char, (void *) label_char);

            //free(tmp_char);
            //free(label_char);

            num_labels ++;
        } else if(instruction->bin == 0) {
            running = false;
        }

        // Update PC
        pc += 4;

    }

    set_max_pc(pc);
}


void dis_scan_terminate() {

    //TODO: :(

    /*for (int i = 0; i < num_of_labels; i++) {
        free(labels[i]);
        free(label_addresses[i]);
    } */

    //hashmap_free(dis_label_map);

}


void dis_print_label(uint32_t address){
    char* addr_char = malloc(sizeof(char));

    gen_int(addr_char, address);

    char* res_label;

    res_label = (char*) hashmap_get(dis_label_map, addr_char);

    if (res_label != NULL) {
        concat(res_label, "\n");
        file_write(res_label);
    }

    free(addr_char);
}

char* dis_get_label(uint32_t address){

    char* addr_char = calloc(0, sizeof(char));

    gen_int(addr_char, address);

    return (char*) hashmap_get(dis_label_map, addr_char);
}
