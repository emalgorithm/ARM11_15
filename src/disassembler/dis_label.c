
#include "../assembler/util/hashmap.h"
#include "../emulator/branch.h"
#include "../emulator/reader.h"
#include "../emulator/arm11.h"
#include "dis_label.h"
#include "dis_exec.h"
#include "writer.h"

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


            compute_offset(&instruction->decoded, &offset);

            //Could do in 1 line
            char* tmp_char = malloc(sizeof(char));
            sprintf(tmp_char, "%d", (pc + offset + 8));

            char* label_char = malloc(sizeof(char));
            sprintf(label_char, "label_%d", num_labels);

            hashmap_put (dis_label_map, tmp_char, (void *) label_char);

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

    /*for (int i = 0; i < num_of_labels; i++) {
        free(labels[i]);
        free(label_addresses[i]);
    } */

}


void dis_print_label(uint32_t address){
    char* addr_char = malloc(sizeof(char));
    sprintf(addr_char, "%d", address);

    char* res_label;

    res_label = (char*) hashmap_get(dis_label_map, addr_char);

    if (res_label != NULL) {
        char* write_char = malloc(sizeof(char));
        sprintf(write_char, "%s:\n", res_label);
        file_write(write_char);
    }
}

char* dis_get_label(uint32_t address){
    char* addr_char = malloc(sizeof(char));
    sprintf(addr_char, "%d", address);

    return (char*) hashmap_get(dis_label_map, addr_char);
}
