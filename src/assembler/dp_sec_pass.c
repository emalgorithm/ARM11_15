#include "dp_sec_pass.h"

func_map_t dp_rd_map;
func_map_t dp_rn_map;

map_t shift_map;
map_t opcode_map;
map_t dp_s_bit_map;

static const int LSL_VAL = 0;
static const int LSR_VAL = 1;
static const int ASR_VAL = 2;
static const int ROR_VAL = 3;

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

struct op2_imm {
    uint32_t imm : 8;
    uint32_t rot : 4;
};

struct op2_reg {
    uint32_t rm        : 4;
    uint32_t bit4      : 1;
    uint32_t sh_ty     : 2;
    uint32_t shift_val : 5;
};

union op2_gen {
    struct op2_reg reg_op;
    struct op2_imm imm_op;
};

void proc_dp_instr (char*, union decoded_instr*);

void dp_set_rd (char*, union decoded_instr*);
void dp_set_not_rd (char*, union decoded_instr*);
void dp_set_rn (char*, union decoded_instr*);
void dp_set_not_rn (char*, union decoded_instr*);
void dp_lsl (char*, union decoded_instr*);

void dp_set_op2 (char*, union decoded_instr*);

void generate_dp_maps () {
    dp_rd_map = hashmap_new();
    dp_rn_map = hashmap_new();

    opcode_map = func_hashmap_new();
    dp_s_bit_map = func_hashmap_new();
    shift_map = func_hashmap_new();

    hashmap_put (opcode_map, "add", (void *) &ADD_OPCODE);
    hashmap_put (opcode_map, "sub", (void *) &SUB_OPCODE);
    hashmap_put (opcode_map, "rsb", (void *) &RSB_OPCODE);
    hashmap_put (opcode_map, "and", (void *) &AND_OPCODE);
    hashmap_put (opcode_map, "eor", (void *) &EOR_OPCODE);
    hashmap_put (opcode_map, "orr", (void *) &ORR_OPCODE);
    hashmap_put (opcode_map, "mov", (void *) &MOV_OPCODE);
    hashmap_put (opcode_map, "tst", (void *) &TST_OPCODE);
    hashmap_put (opcode_map, "teq", (void *) &TEQ_OPCODE);
    hashmap_put (opcode_map, "cmp", (void *) &CMP_OPCODE);
    hashmap_put (opcode_map, "lsl", (void *) &MOV_OPCODE);

    hashmap_put (dp_s_bit_map, "add", (void *) &ADD_S_BIT);
    hashmap_put (dp_s_bit_map, "sub", (void *) &SUB_S_BIT);
    hashmap_put (dp_s_bit_map, "rsb", (void *) &RSB_S_BIT);
    hashmap_put (dp_s_bit_map, "and", (void *) &AND_S_BIT);
    hashmap_put (dp_s_bit_map, "eor", (void *) &EOR_S_BIT);
    hashmap_put (dp_s_bit_map, "orr", (void *) &ORR_S_BIT);
    hashmap_put (dp_s_bit_map, "mov", (void *) &MOV_S_BIT);
    hashmap_put (dp_s_bit_map, "tst", (void *) &TST_S_BIT);
    hashmap_put (dp_s_bit_map, "teq", (void *) &TEQ_S_BIT);
    hashmap_put (dp_s_bit_map, "cmp", (void *) &CMP_S_BIT);
    hashmap_put (dp_s_bit_map, "lsl", (void *) &MOV_S_BIT);

    func_hashmap_put (dp_rd_map, "add", dp_set_rd);
    func_hashmap_put (dp_rd_map, "sub", dp_set_rd);
    func_hashmap_put (dp_rd_map, "rsb", dp_set_rd);
    func_hashmap_put (dp_rd_map, "and", dp_set_rd);
    func_hashmap_put (dp_rd_map, "eor", dp_set_rd);
    func_hashmap_put (dp_rd_map, "orr", dp_set_rd);
    func_hashmap_put (dp_rd_map, "mov", dp_set_rd);
    func_hashmap_put (dp_rd_map, "tst", dp_set_not_rd);
    func_hashmap_put (dp_rd_map, "teq", dp_set_not_rd);
    func_hashmap_put (dp_rd_map, "cmp", dp_set_not_rd);
    func_hashmap_put (dp_rd_map, "lsl", dp_set_rd);

    func_hashmap_put (dp_rn_map, "add", dp_set_rn);
    func_hashmap_put (dp_rn_map, "sub", dp_set_rn);
    func_hashmap_put (dp_rn_map, "rsb", dp_set_rn);
    func_hashmap_put (dp_rn_map, "and", dp_set_rn);
    func_hashmap_put (dp_rn_map, "eor", dp_set_rn);
    func_hashmap_put (dp_rn_map, "orr", dp_set_rn);
    func_hashmap_put (dp_rn_map, "mov", dp_set_not_rn);
    func_hashmap_put (dp_rn_map, "tst", dp_set_rn);
    func_hashmap_put (dp_rn_map, "teq", dp_set_rn);
    func_hashmap_put (dp_rn_map, "cmp", dp_set_rn);
    func_hashmap_put (dp_rn_map, "lsl", dp_lsl);

    hashmap_put (shift_map, "lsl", (void *) &LSL_VAL);
    hashmap_put (shift_map, "lsr", (void *) &LSR_VAL);
    hashmap_put (shift_map, "asr", (void *) &ASR_VAL);
    hashmap_put (shift_map, "ror", (void *) &ROR_VAL);
}

// -----------------------------------------------------------------------

void proc_dp_instr(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    // Set Cond Field
    dp_instr->cond = 0xE;


    // Set Id Field
    dp_instr->_id = 0x0;


    // Set S Field
    dp_instr->set_cond = *((uint8_t *) hashmap_get(dp_s_bit_map, dp_char));

    // Set OpCode

    dp_instr->op_code = *((uint8_t *) hashmap_get(opcode_map, dp_char));

    // Continue
    func_hashmap_get(dp_rd_map, dp_char)(dp_char, instruction);
}

// -----------------------------------------------------------------------

void dp_set_rd(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    // Set Rd register
    dp_instr->rd = tokreg();

    // Continue
    func_hashmap_get(dp_rn_map, dp_char)(dp_char, instruction);
}

void dp_set_not_rd(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    // No Rd Register
    dp_instr->rd = 0x0;

    // Continue
    dp_set_op2(dp_char, instruction);
}

void dp_set_rn(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    // Set Rn register
    dp_instr->rn = tokreg();

    // Continue
    dp_set_op2(dp_char, instruction);
}

void dp_set_not_rn(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    // No Rn Register
    dp_instr->rn = 0x0;

    // Continue
    dp_set_op2(dp_char, instruction);
}

void dp_lsl(char* dp_char, union decoded_instr* instruction) {

    // Horrible Duplication Ahead
    struct dp_instr* dp_instr = &instruction->dp;

    // No Rn Register
    dp_instr->rn = 0x0;

    union op2_gen* op2 = calloc(1, sizeof(union op2_gen));

    struct op2_reg* op2_reg = &op2->reg_op;

    enum operand_type DEFAULT = NONE;
    enum operand_type* op_type = &DEFAULT;

    tokop(op_type);

    // Shifted register is Rd
    op2_reg->rm = dp_instr->rd;
    // Shift by Imm Value
    op2_reg->bit4 = 0;
    // Get shift amount
    op2_reg->shift_val = tokimm();
    dp_instr->op2 = *((int *) op2);

    free(op2);

    write(instruction);

}

// ------------------------------------------------------------------

void dp_set_op2(char* dp_char, union decoded_instr* instruction) {
    struct dp_instr* dp_instr = &instruction->dp;

    union op2_gen* op2 = calloc(1, sizeof(union op2_gen));

    enum operand_type DEFAULT = NONE;
    enum operand_type* op_type = &DEFAULT;

    tokop(op_type);

    switch (*op_type) {

        case IMMEDIATE: ;
            // Union is of type op2_imm
            struct op2_imm* op2_imm = &op2->imm_op;
            // Set I Bit
            dp_instr->imm_op = 0x1;
            // Improper Call to tokimm. Must try rotation.
            op2_imm->imm = tokimm();
            op2_imm->rot = 0x0;
            break;
        case SHIFT_REG: ;
            // Union is of type op2_reg
            struct op2_reg* op2_reg = &op2->reg_op;
            // Not set I Bit
            dp_instr->imm_op = 0x0;
            // Set Rm
            op2_reg->rm = tokreg();

            enum operand_type DEFAULT2 = NONE;
            enum operand_type* sh_op_type = &DEFAULT2;

            op2_reg->sh_ty = * ((int *)hashmap_get(shift_map, tokshift(sh_op_type)));

            switch (*sh_op_type) {
                case IMMEDIATE: ;
                    op2_reg->bit4 = 0;
                    op2_reg->shift_val = tokimm();
                    break;
                case SHIFT_REG: ;
                    op2_reg->bit4 = 1;
                    // Rotate left register to have empty bit7
                    op2_reg->shift_val = tokreg() << 1;
                    break;
                default:
                assert(false);
                    break;
            }
            break;
        default:
        assert(false);
            break;
    }
    dp_instr->op2 = *((int *) op2);

    free(op2);

    write(instruction);
}
