#include "dp_sec_pass.h"
#include "util/hashmap.h"
#include "util/func_hashmap.h"
#include "bwriter.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "util/tokeniser.h"
#include "util/shift_map.h"
#include "../util/binutils.h"
#include "../emulator/util/shift_reg.h"

#define MAX_BITS (32)
#define IMM_BIT (7)
#define IMM_LEN (8)
#define MIN_ROT (2)
#define MAX_ROT (17)

func_map_t dp_rd_map;
func_map_t dp_rn_map;

map_t opcode_map;
map_t dp_s_bit_map;

static const uint32_t ADD_OPCODE = 4;
static const uint32_t SUB_OPCODE = 2;
static const uint32_t RSB_OPCODE = 3;
static const uint32_t AND_OPCODE = 0;
static const uint32_t EOR_OPCODE = 1;
static const uint32_t ORR_OPCODE = 12;
static const uint32_t MOV_OPCODE = 13;
static const uint32_t TST_OPCODE = 8;
static const uint32_t TEQ_OPCODE = 9;
static const uint32_t CMP_OPCODE = 10;

static const uint32_t ADD_S_BIT = 0;
static const uint32_t SUB_S_BIT = 0;
static const uint32_t RSB_S_BIT = 0;
static const uint32_t AND_S_BIT = 0;
static const uint32_t EOR_S_BIT = 0;
static const uint32_t ORR_S_BIT = 0;
static const uint32_t MOV_S_BIT = 0;
static const uint32_t TST_S_BIT = 1;
static const uint32_t TEQ_S_BIT = 1;
static const uint32_t CMP_S_BIT = 1;

void proc_dp_instr (char*, union decoded_instr*);

static void dp_set_rd (char*, union decoded_instr*);
static void dp_set_not_rd (char*, union decoded_instr*);
static void dp_set_rn (char*, union decoded_instr*);
static void dp_set_not_rn (char*, union decoded_instr*);
static void dp_lsl (char*, union decoded_instr*);

static void dp_set_op2 (char*, union decoded_instr*);

void generate_dp_maps (void) {
    dp_rd_map = hashmap_new();
    dp_rn_map = hashmap_new();

    opcode_map = func_hashmap_new();
    dp_s_bit_map = func_hashmap_new();

    hashmap_put(opcode_map, "add", (void *) &ADD_OPCODE);
    hashmap_put(opcode_map, "sub", (void *) &SUB_OPCODE);
    hashmap_put(opcode_map, "rsb", (void *) &RSB_OPCODE);
    hashmap_put(opcode_map, "and", (void *) &AND_OPCODE);
    hashmap_put(opcode_map, "eor", (void *) &EOR_OPCODE);
    hashmap_put(opcode_map, "orr", (void *) &ORR_OPCODE);
    hashmap_put(opcode_map, "mov", (void *) &MOV_OPCODE);
    hashmap_put(opcode_map, "tst", (void *) &TST_OPCODE);
    hashmap_put(opcode_map, "teq", (void *) &TEQ_OPCODE);
    hashmap_put(opcode_map, "cmp", (void *) &CMP_OPCODE);
    hashmap_put(opcode_map, "lsl", (void *) &MOV_OPCODE);

    hashmap_put(dp_s_bit_map, "add", (void *) &ADD_S_BIT);
    hashmap_put(dp_s_bit_map, "sub", (void *) &SUB_S_BIT);
    hashmap_put(dp_s_bit_map, "rsb", (void *) &RSB_S_BIT);
    hashmap_put(dp_s_bit_map, "and", (void *) &AND_S_BIT);
    hashmap_put(dp_s_bit_map, "eor", (void *) &EOR_S_BIT);
    hashmap_put(dp_s_bit_map, "orr", (void *) &ORR_S_BIT);
    hashmap_put(dp_s_bit_map, "mov", (void *) &MOV_S_BIT);
    hashmap_put(dp_s_bit_map, "tst", (void *) &TST_S_BIT);
    hashmap_put(dp_s_bit_map, "teq", (void *) &TEQ_S_BIT);
    hashmap_put(dp_s_bit_map, "cmp", (void *) &CMP_S_BIT);
    hashmap_put(dp_s_bit_map, "lsl", (void *) &MOV_S_BIT);

    func_hashmap_put(dp_rd_map, "add", dp_set_rd);
    func_hashmap_put(dp_rd_map, "sub", dp_set_rd);
    func_hashmap_put(dp_rd_map, "rsb", dp_set_rd);
    func_hashmap_put(dp_rd_map, "and", dp_set_rd);
    func_hashmap_put(dp_rd_map, "eor", dp_set_rd);
    func_hashmap_put(dp_rd_map, "orr", dp_set_rd);
    func_hashmap_put(dp_rd_map, "mov", dp_set_rd);
    func_hashmap_put(dp_rd_map, "tst", dp_set_not_rd);
    func_hashmap_put(dp_rd_map, "teq", dp_set_not_rd);
    func_hashmap_put(dp_rd_map, "cmp", dp_set_not_rd);
    func_hashmap_put(dp_rd_map, "lsl", dp_set_rd);

    func_hashmap_put(dp_rn_map, "add", dp_set_rn);
    func_hashmap_put(dp_rn_map, "sub", dp_set_rn);
    func_hashmap_put(dp_rn_map, "rsb", dp_set_rn);
    func_hashmap_put(dp_rn_map, "and", dp_set_rn);
    func_hashmap_put(dp_rn_map, "eor", dp_set_rn);
    func_hashmap_put(dp_rn_map, "orr", dp_set_rn);
    func_hashmap_put(dp_rn_map, "mov", dp_set_not_rn);
    func_hashmap_put(dp_rn_map, "tst", dp_set_rn);
    func_hashmap_put(dp_rn_map, "teq", dp_set_rn);
    func_hashmap_put(dp_rn_map, "cmp", dp_set_rn);
    func_hashmap_put(dp_rn_map, "lsl", dp_lsl);
}

void free_dp_maps(void) {
    hashmap_free(dp_rd_map);
    hashmap_free(dp_rn_map);
    hashmap_free(opcode_map);
    hashmap_free(dp_s_bit_map);
}


// -----------------------------------------------------------------------

void proc_dp_instr(char* dp_char, union decoded_instr* instruction) {

    // Set Cond Field
    instruction->dp.cond = 0xE;


    // Set Id Field
    instruction->dp._id = 0x0;


    // Set S Field
    instruction->dp.set_cond = *((uint8_t *) hashmap_get(dp_s_bit_map, dp_char));

    // Set OpCode

    instruction->dp.op_code = *((uint8_t *) hashmap_get(opcode_map, dp_char));

    // Set Rd rergister
    func_hashmap_get(dp_rd_map, dp_char)(dp_char, instruction);

    // Set Rn register
    func_hashmap_get(dp_rn_map, dp_char)(dp_char, instruction);
}

// -----------------------------------------------------------------------

static void dp_set_rd(char* dp_char, union decoded_instr* instruction) {

    // Set Rd register
    instruction->dp.rd = tokreg();
}

static void dp_set_not_rd(char* dp_char, union decoded_instr* instruction) {

    // No Rd Register
    instruction->dp.rd = 0x0;
}

static void dp_set_rn(char* dp_char, union decoded_instr* instruction) {

    // Set Rn register
    instruction->dp.rn = tokreg();

    // Set Operand2
    dp_set_op2(dp_char, instruction);
}

static void dp_set_not_rn(char* dp_char, union decoded_instr* instruction) {

    // No Rn Register
    instruction->dp.rn = 0x0;

    // Set Operand2
    dp_set_op2(dp_char, instruction);
}

static void dp_lsl(char* dp_char, union decoded_instr* instruction) {

    // No Rn Register
    instruction->dp.rn = 0x0;

    union op2_gen* op2 = calloc(1, sizeof(union op2_gen));

    enum operand_type DEFAULT = NONE;
    enum operand_type* op_type = &DEFAULT;

    tokop(op_type);

    // Shifted register is Rd
    op2->reg_op.rm = instruction->dp.rd;
    // Shift by Imm Value
    op2->reg_op.bit4 = 0;
    // Get shift amount
    op2->reg_op.shift_val = tokimm();
    instruction->dp.op2 = *((int *) op2);

    free(op2);
}

static int rotate (uint32_t val, struct op2_imm* op2_imm) {

    for (int i = 0; i < MAX_ROT; i ++) {

        uint32_t last_bits = get_bits(val, IMM_BIT, IMM_LEN);

        // Debugging print statements
        //printf("---\n");
        //printf("%x\n", val);
        //printf("%x\n", last_bits);

        if (last_bits == val) {
            // If the value is contained in the last 8 bits
            op2_imm->imm = last_bits;
            op2_imm->rot = i;
            return 0;
        } else {
            // Otherwise rotate LEFT 2
            uint32_t left_shift = val << MIN_ROT;
            uint32_t right_shift = val >> (MAX_BITS - MIN_ROT);
            val = left_shift | right_shift;
        }
    }

    printf("\nInvalid Immediate Operand Rotation\n");
    assert(false);

}

// ------------------------------------------------------------------

static void dp_set_op2(char* dp_char, union decoded_instr* instruction) {

    // Allocate space for generic struct for Operand2
    union op2_gen* op2 = calloc(1, sizeof(union op2_gen));

    enum operand_type DEFAULT = NONE;
    enum operand_type* op_type = &DEFAULT;

    tokop(op_type);

    switch (*op_type) {

    case IMMEDIATE:
        // Operand 2 is a Rotated Immediate Operand
        // Set I Bit
        instruction->dp.imm_op = 0x1;
        // Calculate rotation and assign it to imm_op
        rotate(tokimm(), &op2->imm_op);
        break;
    case SHIFT_REG:
        // Operand 2 is a Shifted Register
        // Not set I Bit
        instruction->dp.imm_op = 0x0;
        // Set Rm
        op2->reg_op.rm = toksignedreg(NULL);

        enum operand_type DEFAULT2 = NONE;
        enum operand_type* sh_op_type = &DEFAULT2;

        op2->reg_op.sh_ty = tokshift(sh_op_type);

        switch (*sh_op_type) {
        case NONE:
            // The third register is NOT shifted
            op2->reg_op.bit4 = 0;
            op2->reg_op.shift_val = 0;
            break;
        case IMMEDIATE:
            // Register Rm is Shifted by an Immediate Value
            op2->reg_op.bit4 = 0;
            op2->reg_op.shift_val = tokimm();
            break;
        case SHIFT_REG:
            // Register Rm is shifted by the cont. of another register
            op2->reg_op.bit4 = 1;
            // Rotate left register to have empty bit7
            op2->reg_op.shift_val = toksignedreg(NULL) << 1;
            break;
        default:
            // sh_op_type should ALWAYS be NONE, IMMEDIATE or SHIFT_REG
            assert(false);
            break;
        }
        break;
    default:
        // op_type Should always either IMMEDIATE of SHIFT_REG
        assert(false);
        break;
    }
    // Assign op2 to the relevant field int he instruction
    instruction->dp.op2 = *((int *) op2);

    free(op2);
}
