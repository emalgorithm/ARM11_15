#include "dis_br.h"
#include "../emulator/pipeline.h"
#include "../emulator/branch.h"
#include "dis_label.h"
#include "dis_exec.h"
#include "writer.h"
#include "util/str_util.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>

void dis_br_instr(char* path, union decoded_instr* instruction) {

    char* res_char = calloc(0, sizeof(char));

    char* label_char;
    char* cond_char;

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

    concat(res_char, "b");
    concat(res_char, cond_char);
    concat(res_char, " ");
    concat(res_char, label_char);
    concat(res_char, "\n");



    file_write(res_char);

    free(label_char);
    free(res_char);
}
