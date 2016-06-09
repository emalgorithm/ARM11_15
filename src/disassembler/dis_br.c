#include "dis_br.h"
#include "../emulator/pipeline.h"
#include "../emulator/branch.h"
#include "dis_label.h"
#include "dis_exec.h"
#include "writer.h"

#include <assert.h>
#include <stdbool.h>

void dis_br_instr(char* path, union decoded_instr* instruction) {

    char* cond_char = malloc(sizeof(char));
    char* label_char = malloc(sizeof(char));

    switch(instruction->br.cond) {
        case eq:
            cond_char = "eq";
            break;
        case ne:
            cond_char = "ne";
            break;
        case ge:
            cond_char = "ge";
            break;
        case lt:
            cond_char = "lt";
            break;
        case gt:
            cond_char = "gt";
            break;
        case le:
            cond_char = "le";
            break;
        case al:
            cond_char = "al";
            break;
        default:
            assert(false);
    }

    int32_t off = 0;

    compute_offset(instruction, &off);

    label_char = dis_get_label(get_pc() + 8 + off);

    char* res_char = malloc(sizeof(char));

    sprintf(res_char, "b%s %s\n", cond_char, label_char);

    file_write(res_char);


}
