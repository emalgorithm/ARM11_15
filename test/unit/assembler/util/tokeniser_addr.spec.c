#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../minunit.h"
#include "../../../../src/assembler/util/tokeniser.h"

#define SOURCE "../test/unit/assembler/util/sdt_test.s"

static char spec[] = "tokeniser addressing";

int tests_run = 0;

void setup() {
}

void tear_down() {
}

static int test_addr_mov() {
    toknext();

    int base = tokreg();
    mu_assert(base == 1);

    enum addressing_mode *mode = malloc(sizeof(enum addressing_mode));
    enum operand_type *op = malloc(sizeof(enum operand_type));

    long address = tokaddr(mode, op);

    mu_assert(address == 0xf1);
    mu_assert(*op == ADDRESS);
    mu_assert(*mode == MOV);

    free(mode);
    free(op);

    return 0;
}

static int test_addr_big() {
    toknext();

    int dest = tokreg();
    mu_assert(dest == 1);

    enum addressing_mode *mode = malloc(sizeof(enum addressing_mode));
    enum operand_type *op = malloc(sizeof(enum operand_type));

    long address = tokaddr(mode, op);

    mu_assert(address == 0xffff);
    mu_assert(*op == ADDRESS);
    mu_assert(*mode == PRE);

    free(mode);
    free(op);

    return 0;
}

static int test_addr_base() {
    toknext();

    int dest = tokreg();
    mu_assert(dest == 1);

    enum addressing_mode *mode = malloc(sizeof(enum addressing_mode));
    enum operand_type *op = malloc(sizeof(enum operand_type));

    long base = tokaddr(mode, op);

    mu_assert(base == 2);
    mu_assert(*op == NONE);
    mu_assert(*mode == PRE);

    free(mode);
    free(op);

    return 0;
}

static int test_addr_base_with_imm_offset() {
    toknext();

    int dest = tokreg();
    mu_assert(dest == 1);

    enum addressing_mode *mode = malloc(sizeof(enum addressing_mode));
    enum operand_type *op = malloc(sizeof(enum operand_type));

    long base = tokaddr(mode, op);

    mu_assert(base == 2);
    mu_assert(*op == IMMEDIATE);
    mu_assert(*mode == PRE);

    free(mode);
    free(op);

    mu_assert(tokimm() == 0xffa);

    return 0;
}

static int test_addr_base_with_reg_offset() {
    toknext();

    int dest = tokreg();
    mu_assert(dest == 1);

    enum addressing_mode *mode = malloc(sizeof(enum addressing_mode));
    enum operand_type *op = malloc(sizeof(enum operand_type));

    long base = tokaddr(mode, op);

    mu_assert(base == 2);
    mu_assert(*op == SHIFT_REG);
    mu_assert(*mode == PRE);

    free(mode);
    free(op);

    bool *positive = malloc(sizeof(bool));

    mu_assert(toksignedreg(positive) == 3);
    mu_assert(*positive);
    free(positive);

    enum operand_type *shift = malloc(sizeof(enum operand_type));
    char *sname = tokshift(shift);

    mu_assert(strcmp(sname, "lsl") == 0);
    mu_assert(*shift == IMMEDIATE);

    mu_assert(tokimm() == 0x3);

    return 0;
}

static int test_addr_base_with_reg_offset_shift() {
    toknext();

    int dest = tokreg();
    mu_assert(dest == 1);

    enum addressing_mode *mode = malloc(sizeof(enum addressing_mode));
    enum operand_type *op = malloc(sizeof(enum operand_type));

    long base = tokaddr(mode, op);

    mu_assert(base == 2);
    mu_assert(*op == SHIFT_REG);
    mu_assert(*mode == PRE);

    free(mode);
    free(op);

    bool *positive = malloc(sizeof(bool));

    mu_assert(toksignedreg(positive) == 3);
    mu_assert(*positive);
    free(positive);

    enum operand_type *shift = malloc(sizeof(enum operand_type));
    char *sname = tokshift(shift);

    mu_assert(strcmp(sname, "asr") == 0);
    mu_assert(*shift == SHIFT_REG);

    mu_assert(toksignedreg(NULL) == 4);

    return 0;
}

static int test_addr_neg_pc() {
    toknext();

    int dest = tokreg();
    mu_assert(dest == 1);

    enum addressing_mode *mode = malloc(sizeof(enum addressing_mode));
    enum operand_type *op = malloc(sizeof(enum operand_type));

    long base = tokaddr(mode, op);

    mu_assert(base == 2);
    mu_assert(*op == SHIFT_REG);
    mu_assert(*mode == PRE);

    free(mode);
    free(op);

    bool *positive = malloc(sizeof(bool));

    mu_assert(toksignedreg(positive) == 15);
    mu_assert(!(*positive));
    free(positive);

    enum operand_type *shift = malloc(sizeof(enum operand_type));
    char *sname = tokshift(shift);

    mu_assert(strcmp(sname, "lsl") == 0);
    mu_assert(*shift == IMMEDIATE);

    mu_assert(tokimm() == 13);

    return 0;
}

static int test_post_idx() {
    //ldr r1, [r2], #0x1516
    toknext();

    int dest = tokreg();
    mu_assert(dest == 1);

    enum addressing_mode *mode = malloc(sizeof(enum addressing_mode));
    enum operand_type *op = malloc(sizeof(enum operand_type));

    long base = tokaddr(mode, op);

    mu_assert(base == 2);
    mu_assert(*op == IMMEDIATE);
    mu_assert(*mode == POST);

    free(mode);
    free(op);

    mu_assert(tokimm() == 0x1516);

    return 0;
}

static int test_post_idx_shift() {
    // ldr r1, [PC], -r4, lsr r5
    toknext();

    int dest = tokreg();
    mu_assert(dest == 1);

    enum addressing_mode *mode = malloc(sizeof(enum addressing_mode));
    enum operand_type *op = malloc(sizeof(enum operand_type));

    long base = tokaddr(mode, op);

    mu_assert(base == 15);
    mu_assert(*op == SHIFT_REG);
    mu_assert(*mode == POST);

    free(mode);
    free(op);

    bool *positive = malloc(sizeof(bool));

    mu_assert(toksignedreg(positive) == 4);
    mu_assert(!(*positive));
    free(positive);

    enum operand_type *shift = malloc(sizeof(enum operand_type));
    char *sname = tokshift(shift);

    mu_assert(strcmp(sname, "lsr") == 0);
    mu_assert(*shift == SHIFT_REG);

    mu_assert(toksignedreg(NULL) == 5);

    return 0;
}

static int test_all() {
    tokinit(SOURCE);

    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_addr_mov);
    mu_run_test(test_addr_big);
    mu_run_test(test_addr_base);
    mu_run_test(test_addr_base_with_imm_offset);
    mu_run_test(test_addr_base_with_reg_offset);
    mu_run_test(test_addr_base_with_reg_offset_shift);
    mu_run_test(test_addr_neg_pc);
    mu_run_test(test_post_idx);
    mu_run_test(test_post_idx_shift);

    tokdestroy();

    return 0;
}

int main(int argc, char **argv) {

    int res = test_all();

    if (res != 0) {
        printf("TEST #%d FAILED.\n", res);
    } else {
        printf("%d passed\n", tests_run);
    }

    return res != 0;
}
