#include <stdio.h>
#include <stdint.h>
#include "../minunit.h"
#include "../../../src/emulator/data_processing.h"
#include "../../../src/emulator/arm11.h"

static char spec[] = "data_processing";

int tests_run = 0;

/*TODO: Add tests for setconditions*/

void setup() {
    // arm11 state init
    initialize();

    set_register(0, 0);
    set_register(1, 0);
}

void tear_down() {}

static struct dp_instr* gen_instr (int op_code) {

    struct dp_instr *instruction;
    instruction = malloc(sizeof(struct dp_instr));

    instruction->imm_op = 1;
    instruction->op_code = op_code;
    instruction->set_cond = 0;
    instruction->op1 = 0;
    instruction->dest = 1;
    instruction->op2 = 6;

    return instruction;
}

static void run_gen_instr (int op_code) {
    struct dp_instr *gen_instruction = gen_instr(op_code);
    dp_exec((void*) gen_instruction);
}

/* TEST CASE LEGEND
 * test_ : defines the test function
 * and_  : type of function tested
 * imm_  : immediate operand is tested
 * nr    : no rotation involved
 * wr    : with rotation involved
 */

// AND TESTS
//1
static int test_and_imm_nr() {
    /* 0x4 AND 0x6 = 0x4 */
    set_register(0, 0x4);

    run_gen_instr(0);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x4);
    return 0;
}

//2
static int test_and_imm_wr_1() {
    /* 0x20C = 0x0C Rotated 0x2 = 0x3
     * 0x6 AND 0x3 = 0x2*/
    set_register(0, 0x6);

    struct dp_instr *gen_instruction = gen_instr(0);
    gen_instruction->op2 = 0x20C;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x2);
    return 0;
}

//3
static int test_and_imm_wr_2() {
    /* 0x408 = 0x08 Rotated 0x4 = 0x80000000
     * 0x80000000 AND 0x80000000 = 0x80000000 */
    set_register(0, 0x80000000);

    struct dp_instr *gen_instruction = gen_instr(0);
    gen_instruction->op2 = 0x408;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x80000000);
    return 0;
}

// EOR TESTS
//4
static int test_eor_imm_nr() {
    /* 0xC EOR 0x6 = 0xA */
    set_register(0, 12);

    run_gen_instr(1);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 10);
    return 0;
}

//5
static int test_eor_imm_wr_1() {
    /* 0x20C = 0x0C Rotated 0x2 = 0x3
     * 0x8 EOR 0x3 = 0xB */
    set_register(0, 0x8);

    struct dp_instr *gen_instruction = gen_instr(1);
    gen_instruction->op2 = 0x20C;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xB);
    return 0;
}

//6
static int test_eor_imm_wr_2() {
    /* 0x408 = 0x08 Rotated 0x4 = 0x80000000
     * 0x70000000 EOR 0x80000000 = 0xF0000000 */
    set_register(0, 0x70000000);

    struct dp_instr *gen_instruction = gen_instr(1);
    gen_instruction->op2 = 0x408;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xF0000000);
    return 0;
}

// SUB TESTS
//7
static int test_sub_imm_nr_1() {
    /* 0xA SUB 0x6 = 0x4 */
    set_register(0, 10);

    run_gen_instr(2);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 4);
    return 0;
}

//8
static int test_sub_imm_nr_2() {
    /* 0xFFFFFFFB SUB 0x6 = 0xFFFFFFF5
     * -5 SUB 6 = -11 */
    set_register(0, 0xFFFFFFFB);

    run_gen_instr(2);

    uint32_t reg_content = get_register(1);
    mu_assert (reg_content == 0xFFFFFFF5);
    return 0;
}

//9
static int test_sub_imm_wr_1() {
    /* 0x20C = 0x0C Rotated 0x2 = 0x3
     * 0x8 SUB 0x3 = 0x5 */
    set_register(0, 0x8);

    struct dp_instr *gen_instruction = gen_instr(2);
    gen_instruction->op2 = 0x20C;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x5);
    return 0;
}

//10
static int test_sub_imm_wr_2() {
    /* 0x404 = 0x04 Rotated 0x4 = 0x40000000
     * 0x70000000 SUB 0x40000000 = 0x30000000 */
    set_register(0, 0x70000000);

    struct dp_instr *gen_instruction = gen_instr(2);
    gen_instruction->op2 = 0x404;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x30000000);
    return 0;
}

// RSB TESTS
//11
static int test_rsb_imm_nr_1() {
    /* 0x3 RSB 0x6 = 0x3 */
    set_register(0, 0x3);

    run_gen_instr(3);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x3);
    return 0;
}

//12
static int test_rsb_imm_nr_2() {
    /* 0xA RSB 0x6 = 0xFFFFFFFC
     * 10 RSB 6 = -4 */
    set_register(0, 0xA);

    run_gen_instr(3);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == (uint32_t) 0xFFFFFFFC);
    return 0;
}

//13
static int test_rsb_imm_wr_1() {
    /* 0x20C = 0x0C Rotated 0x2 = 0x3
     * 0x8 RSP 0x3 = 0xFFFFFFFB */
    set_register(0, 0x8);

    struct dp_instr *gen_instruction = gen_instr(3);
    gen_instruction->op2 = 0x20C;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xFFFFFFFB);
    return 0;
}

//14
static int test_rsb_imm_wr_2() {
    /* 0x408 = 0x08 Rotated 0x4 = 0x80000000
     * 0x70000000 RSB 0x80000000 = 0x10000000 */
    set_register(0, 0x70000000);

    struct dp_instr *gen_instruction = gen_instr(3);
    gen_instruction->op2 = 0x408;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x10000000);
    return 0;
}

// ADD TESTS
//15
static int test_add_imm_nr() {
    /* 0x2 ADD 0x6 = 0x8 */
    set_register(0, 0x2);

    run_gen_instr(4);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x8);
    return 0;
}

//16
static int test_add_imm_wr_1() {
    /* 0x10E = 0x0E Rotated 0x1 = 0x7
     * 0x8 RSP 0x7 = 0xF */
    set_register(0, 0x8);

    struct dp_instr *gen_instruction = gen_instr(4);
    gen_instruction->op2 = 0x10E;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xF);
    return 0;
}

//17
static int test_add_imm_wr_2() {
    /* 0x30C = 0x0C Rotated 0x3 = 0x80000001
     * 0x70000000 RSB 0x80000000 = 0xF0000000 */
    set_register(0, 0x70000000);

    struct dp_instr *gen_instruction = gen_instr(4);
    gen_instruction->op2 = 0x30C;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xF0000001);
    return 0;
}

// TST TESTS
//18
static int test_tst_imm_nr() {
    /* 0x8 AND 0x6 = 0x4 -- RESULT NOT WRITTEN */
    set_register(0, 0x8);
    set_register(1, 0xF);

    run_gen_instr(5);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xF);
    return 0;
}

//19
static int test_tst_imm_wr() {
    /* 0x408 = 0x08 Rotated 0x4 = 0x80000000
     * 0x80000000 TST 0x80000000 = 0x80000000
     * -- RESULT NOT WRITTEN */
    set_register(0, 0x80000000);
    set_register(1, 0xF);

    struct dp_instr *gen_instruction = gen_instr(5);
    gen_instruction->op2 = 0x408;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xF);
    return 0;
}

// TEQ TESTS
//20
static int test_teq_imm_nr() {
    /* 0xC TEQ 0x6 = 0XA -- RESULT NOT WRITTEN */
    set_register(0, 0xC);
    set_register(1, 0xF);

    run_gen_instr(6);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xF);
    return 0;
}

//21
static int test_teq_imm_wr() {
    /* 0x408 = 0x08 Rotated 0x4 = 0x80000000
     * 0x70000000 TEQ 0x80000000 = 0xF0000000
     * -- RESULT NOT WRITTEN */
    set_register(0, 0x70000000);
    set_register(1, 0xABCD);

    struct dp_instr *gen_instruction = gen_instr(6);
    gen_instruction->op2 = 0x408;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xABCD);
    return 0;
}

// CMP TESTS
//22
static int test_cmp_imm_nr() {
    /* 0xA CMP 0x6 = 0x4 -- RESULT NOT WRITTEN */
    set_register(0, 10);
    set_register(1, 0xF);

    run_gen_instr(7);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xF);
    return 0;
}

//23
static int test_cmp_imm_wr() {
    /* 0x404 = 0x04 Rotated 0x4 = 0x40000000
     * 0x70000000 CMP 0x40000000 = 0x30000000
     * -- RESULT NOT WRITTEN */
    set_register(0, 0x70000000);
    set_register(1, 0xABCD);

    struct dp_instr *gen_instruction = gen_instr(7);
    gen_instruction->op2 = 0x404;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xABCD);
    return 0;
}

// ORR TESTS
//24
static int test_orr_imm_nr() {
    /* 0xC OR 0x6 = 0xE */
    set_register(0, 0xC);

    run_gen_instr(8);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xE);
    return 0;
}

//25
static int test_orr_imm_wr() {
    /* 0x22C = 0x2C Rotated 0x2 = 0xB
     * 0x8 ORR 0xB = 0xB */
    set_register(0, 0x8);

    struct dp_instr *gen_instruction = gen_instr(8);
    gen_instruction->op2 = 0x22C;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xB);
    return 0;
}

// MOV TESTS
//26
static int test_mov_imm_nr() {
    /* 0x0 MOV 0x6 = 0x6 */
    set_register(0, 0x0);

    run_gen_instr(9);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x6);
    return 0;
}

//27
static int test_mov_imm_wr() {
    /* 0x22C = 0x2C Rotated 0x2 = 0xB
     * 0xF ORR 0xB = 0xB */
    set_register(0, 0xF);

    struct dp_instr *gen_instruction = gen_instr(9);
    gen_instruction->op2 = 0x22C;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xB);
    return 0;
}



static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    //1
    mu_run_test(test_and_imm_nr);
    //2
    mu_run_test(test_and_imm_wr_1);
    //3
    mu_run_test(test_and_imm_wr_2);
    //4
    mu_run_test(test_eor_imm_nr);
    //5
    mu_run_test(test_eor_imm_wr_1);
    //6
    mu_run_test(test_eor_imm_wr_2);
    //7
    mu_run_test(test_sub_imm_nr_1);
    //8
    mu_run_test(test_sub_imm_nr_2);
    //9
    mu_run_test(test_sub_imm_wr_1);
    //10
    mu_run_test(test_sub_imm_wr_2);
    //11
    mu_run_test(test_rsb_imm_nr_1);
    //12
    mu_run_test(test_rsb_imm_nr_2);
    //13
    mu_run_test(test_rsb_imm_wr_1);
    //14
    mu_run_test(test_rsb_imm_wr_2);
    //15
    mu_run_test(test_add_imm_nr);
    //16
    mu_run_test(test_add_imm_wr_1);
    //17
    mu_run_test(test_add_imm_wr_2);
    //18
    mu_run_test(test_tst_imm_nr);
    //19
    mu_run_test(test_tst_imm_wr);
    //20
    mu_run_test(test_teq_imm_nr);
    //21
    mu_run_test(test_teq_imm_wr);
    //22
    mu_run_test(test_cmp_imm_nr);
    //23
    mu_run_test(test_cmp_imm_wr);
    //24
    mu_run_test(test_orr_imm_nr);
    //25
    mu_run_test(test_orr_imm_wr);
    //26
    mu_run_test(test_mov_imm_nr);
    //27
    mu_run_test(test_mov_imm_wr);

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
