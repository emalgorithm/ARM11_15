/* Module  : sdt_sec_pass
 *
 * Usage   : Parse SDT instruction from tokeniser
 *
 * Authors : Tencho Tenev
 */

#include <string.h>
#include "sdt_sec_pass.h"
#include "util/tokeniser.h"
#include "util/shift_map.h"
#include "dp_sec_pass.h"
#include "bwriter.h"
#include "second_pass.h"
#include "math.h"

// Unconditional execution of SDT instructions
#define COND 0xe

#define ID 1
#define MOVID 0
#define MOVOPCODE 0xd

#define DATA_SECTION_SIZE 256
static uint32_t *data_section = NULL;
static short data_count = 0;

static void handle_mov(char *token, union decoded_instr *instr);
// static void handle_pre(char *token, decoded_instr *instr);
// static void handle_post(char *token, decoded_instr *instr);

void proc_sdt_init() {
    data_section = calloc(DATA_SECTION_SIZE, sizeof(uint32_t));
}

void proc_sdt_instr(char *token, union decoded_instr *instr) {

    // This can be done because the fields match with DP when MOV
    instr->sdt.cond = COND;
    instr->sdt.rd = tokreg();

    enum addressing_mode *mode = malloc(sizeof(enum addressing_mode));
    enum operand_type *operand = malloc(sizeof(enum operand_type));

    long base = tokaddr(mode, operand);

    if (*mode == MOV) {
        handle_mov(token, instr);
        instr->dp.op2 = base;
    } else {
        instr->sdt._id = ID;
        instr->sdt.load_store = (strcmp(token, "ldr") == 0) ? 1 : 0;
        instr->sdt.up = 0;

        if (*operand == ADDRESS) {
            // get last address, compute offset ...
            // base is the value
            instr->sdt.rn = 0;
            instr->sdt.index_bit = 1; // only pre-indexing
            int la = lastaddr();
            int ca = get_curr_instr_addr();
            int offset = la + 4 * data_count - ca - 8;
            instr->sdt.offset = abs(offset);

            instr->sdt.up = (offset < 0) ? 0 : 1;
            instr->sdt.imm_off = 0;
            instr->sdt.rn = 0xf;
            data_section[data_count++] = base;
        } else {
            instr->sdt.rn = base; // offset base is a register
            instr->sdt.up = 1; // always add instead explicitly set below
            instr->sdt.index_bit = (*mode == PRE) ? 1 : 0; // indexing mode

            if (*operand == NONE) {
                instr->sdt.imm_off = 0;
                instr->sdt.offset = 0;
            } else if (*operand == IMMEDIATE) {
                instr->sdt.imm_off = 0;
                long offset = tokimm();
                instr->sdt.offset = abs(offset);
                if (offset < 0) {
                    instr->sdt.up = 0;
                }
            } else if (*operand == SHIFT_REG) {
                bool *positive = malloc(sizeof(bool));

                struct op2_reg *offset = calloc(1, 32);

                offset->rm = toksignedreg(positive); // bits 0-3 of offset

                if (!positive) {
                    instr->sdt.up = 0;
                }

                free(positive);

                enum operand_type *shift = malloc(sizeof(enum operand_type));

                offset->sh_ty = tokshift(shift); // bits 5-6


                if (*shift == IMMEDIATE) {
                    // bit 4 is 0
                    offset->bit4 = 0;

                    offset->shift_val = tokimm();
                } else {
                    // bit 4 is 1
                    offset->bit4 = 1;
                    offset->shift_val = (toksignedreg(NULL) << 1);
                }

                instr->sdt.offset = *(int *)offset;

                free(offset);
            }
        }
    }

    free(mode);
    free(operand);
}

void write_data_section() {
    for (int i = 0; i < data_count; ++i) {
        bwr_data(data_section[i]);
    }
}

static void handle_mov(char *token, union decoded_instr *instr) {
    instr->dp._id = MOVID;
    instr->dp.op_code = MOVOPCODE;
    instr->dp.imm_op = 1;
    instr->dp.set_cond = 0;
}
