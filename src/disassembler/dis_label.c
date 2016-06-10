
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

struct link_str {
    char* str;
    struct link_str* next;
    struct link_str* prev;
};

static struct link_str* start;
static struct link_str* end;

static void gen_list() {
    start = malloc(sizeof(struct link_str));
    end = malloc(sizeof(struct link_str));

    start->str = "start";
    start->next = end;
    start->prev = NULL;

    end->str = "end";
    end->next = NULL;
    end->prev = start;
}

static void del_list() {
    free(start);
    free(end);
}

static struct link_str* get_start() {
    return start->next;
}

static struct link_str* get_end() {
    return end;
}

static void add_elem(char* elem) {
    struct link_str* curr_end = get_end();

    struct link_str* new_elem = malloc(sizeof(struct link_str));

    struct link_str* curr_prev = curr_end->prev;

    curr_prev->next = new_elem;
    end->prev = new_elem;

    new_elem->str = elem;
    new_elem->prev = curr_prev;
    new_elem->next = curr_end;
}

void dis_scan_init() {
    dis_label_map = hashmap_new();
    gen_list();
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

            //Add elements to a Linked List to free them later

            add_elem(label_char);
            add_elem(tmp_char);

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

    struct link_str* head = get_start();

    struct link_str* curr;

    //Free contents of Linked List
    while ((curr = head) != get_end()) {
        //TODO: Free Memory in HashMap
        //free(curr->str)
        //free(curr);
        head = head->next;
    }

    free(head);

    del_list();

}


void dis_print_label(uint32_t address){
    char* addr_char = calloc(0, sizeof(char));
    char* prnt_label;

    gen_int(addr_char, address);

    char* res_label;

    res_label = (char*) hashmap_get(dis_label_map, addr_char);

    if (res_label != NULL) {
        prnt_label = calloc(sizeof(res_label)+1, sizeof(char));
        strcpy(prnt_label, res_label);

        concat(prnt_label, ":\n");
        file_write(prnt_label);
    }

    free(addr_char);
}

char* dis_get_label(uint32_t address){

    char* addr_char = calloc(0, sizeof(char));

    gen_int(addr_char, address);

    return (char*) hashmap_get(dis_label_map, addr_char);
}
