#include "br_sec_pass.h"
#include "util/tokeniser.h"
#include "label_scanner.h"
#include "second_pass.h"

void proc_br_instr(char* br_char, union decoded_instr *decoded) {
    decoded->br._id = 0x2;

    decoded->br.cond = tokcond(br_char);

    char *label = tokbrlabel();

    uint32_t label_addr = get_label_address(label);
    uint32_t curr_addr = get_curr_instr_addr();

    uint32_t offset = curr_addr - label_addr - 8;

    decoded->br.offset = offset;
}
