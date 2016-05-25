#include <stdio.h>
#include <stdint.h>
#include "../minunit.h"
#include "../../../src/emulator/data_processing.h"
#include "../../../src/emulator/arm11.h"
#include "../../../src/emulator/util/cpsr_flags.h"

static char spec[] = "data_processing";

int tests_run = 0;

void setup() {
    // arm11 state init
    initialize();

    set_register(0, 0);
    set_register(1, 0);
    set_register(2, 0);

    clr_nflag;
    clr_zflag;
    clr_cflag;
    clr_nflag;


}

void tear_down() {}

static struct dp_instr* gen_instr (int op_code) {

    struct dp_instr *instruction;
    instruction = malloc(sizeof(struct dp_instr));

    instruction->imm_op = 1;
    instruction->op_code = op_code;
    instruction->set_cond = 1;
    instruction->rn = 0;
    instruction->rd = 1;
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
 * reg   : no immediate operand = register
 * fl    : checks flags
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

//4
static int test_and_reg_nr() {
    /* 0x4 AND 0x6 = 0x4 */
    set_register(0, 0x4);
    set_register(2, 0x6);

    struct dp_instr *gen_instruction = gen_instr(0);
    gen_instruction->op2 = 0x2;
    gen_instruction->imm_op = 0x0;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x4);
    return 0;
}

//5
static int test_and_reg_wr() {
    /* 0x20C = 0x0C Rotated 0x2 = 0x3
     * 0x6 AND 0x3 = 0x2*/
    set_register(0, 0x6);
    set_register(2, 0xC);

    struct dp_instr *gen_instruction = gen_instr(0);
    gen_instruction->op2 = 0x162;
    gen_instruction->imm_op = 0x0;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x2);
    return 0;
}

//6
static int test_and_imm_nr_fl() {
  set_register(0, 0x0);

  struct dp_instr *gen_instruction = gen_instr(0);
  gen_instruction->op2 = 0x0;
  dp_exec((void*) gen_instruction);

  uint32_t reg_content = get_register(1);
  mu_assert(reg_content == 0x0 && get_zflag == 1 && get_nflag == 0);
  return 0;
}

// EOR TESTS
//7
static int test_eor_imm_nr() {
    /* 0xC EOR 0x6 = 0xA */
    set_register(0, 12);

    run_gen_instr(1);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 10);
    return 0;
}

//8
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

//9
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

//10
static int test_eor_reg_wr() {
  /* 0x20C = 0x0C Rotated 0x2 = 0x3
   * 0x8 EOR 0x3 = 0xB */
  set_register(0, 0x8);
  set_register(1, 0xC);
  set_register(2, 0x2);

  struct dp_instr *gen_instruction = gen_instr(1);
  gen_instruction->op2 = 0x231;
  gen_instruction->imm_op = 0x0;
  dp_exec((void*) gen_instruction);

  uint32_t reg_content = get_register(1);
  mu_assert(reg_content == 0xB);
  return 0;
}

//11
static int test_eor_reg_wr_fl() {
  /* 0x4 EOR 0x0 With Count Flag*/
  set_register(0, 0x4);
  set_register(1, 0x80000000);

  struct dp_instr *gen_instruction = gen_instr(1);
  gen_instruction->op2 = 0x081;
  gen_instruction->imm_op = 0x0;
  dp_exec((void*) gen_instruction);

  uint32_t reg_content = get_register(1);
  mu_assert(reg_content == 0x4 && get_cflag == 1);
  return 0;
}

// SUB TESTS
//12
static int test_sub_imm_nr_1() {
    /* 0xA SUB 0x6 = 0x4 */
    set_register(0, 0xA);

    run_gen_instr(2);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x4);
    return 0;
}

//13
static int test_sub_imm_nr_2() {
    /* 0xFFFFFFFB SUB 0x6 = 0xFFFFFFF5
     * -5 SUB 6 = -11 */
    set_register(0, 0xFFFFFFFB);

    run_gen_instr(2);

    uint32_t reg_content = get_register(1);
    mu_assert (reg_content == 0xFFFFFFF5);
    return 0;
}

//14
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

//15
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

//16
static int test_sub_reg_nr() {
    /* 0x8 SUB 0x3 = 0x5 */
    set_register(0, 0x8);
    set_register(2, 0x3);

    struct dp_instr *gen_instruction = gen_instr(2);
    gen_instruction->op2 = 0x2;
    gen_instruction->imm_op = 0x0;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x5);
    return 0;
}

//17
static int test_sub_reg_wr_fl_1() {
    /* 0x2 SUB 0x3 = 0x0 WITH CARRY */
    set_register(0, 0x80000000);
    set_register(2, 0x80000000);

    struct dp_instr *gen_instruction = gen_instr(2);
    gen_instruction->op2 = 0xC2;
    gen_instruction->imm_op = 0x0;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x40000000 &&  get_nflag == 0);
    return 0;
}

//18
static int test_sub_reg_wr_fl_2() {
    /* 0x20000000 SUB 0x20000001 = 0xFFFFFFF WITH NO CARRY */
    set_register(0, 0x20000000);
    set_register(2, 0x40000002);

    struct dp_instr *gen_instruction = gen_instr(2);
    gen_instruction->op2 = 0xC2;
    gen_instruction->imm_op = 0x0;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xFFFFFFFF && get_cflag == 0 && get_nflag == 1);
    return 0;
}

//19
static int test_sub_imm_nr_fl() {
    /* 0x8 SUB 0x3 = 0x5 */
    set_register(0, 0x1);

    struct dp_instr *gen_instruction = gen_instr(2);
    gen_instruction->op2 = 0x1;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x0 && get_cflag == 1);
    return 0;
}

// RSB TESTS
//20
static int test_rsb_imm_nr_1() {
    /* 0x3 RSB 0x6 = 0x3 */
    set_register(0, 0x3);

    run_gen_instr(3);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x3);
    return 0;
}

//21
static int test_rsb_imm_nr_2() {
    /* 0xA RSB 0x6 = 0xFFFFFFFC
     * 10 RSB 6 = -4 */
    set_register(0, 0xA);

    run_gen_instr(3);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == (uint32_t) 0xFFFFFFFC);
    return 0;
}

//22
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

//23
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

//24
static int test_rsb_reg_nr_fl() {
    /* 0x20000000 SUB 0x20000001 = 0xFFFFFFF WITH CARRY */
    set_register(0, 0x20000000);
    set_register(2, 0x20000000);

    struct dp_instr *gen_instruction = gen_instr(3);
    gen_instruction->op2 = 0x42;
    gen_instruction->imm_op = 0x0;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x0 && get_zflag == 1);
    return 0;
}

//25
static int test_rsb_reg_wr_fl() {
    /* 0x20000001 RSB 0x20000000 = 0xFFFFFFF WITH CARRY */
    set_register(0, 0x20000001);
    set_register(2, 0x40000000);

    struct dp_instr *gen_instruction = gen_instr(3);
    gen_instruction->op2 = 0xC2;
    gen_instruction->imm_op = 0x0;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xFFFFFFFF && get_cflag == 0);
    return 0;
}

// ADD TESTS
//26
static int test_add_imm_nr() {
    /* 0x2 ADD 0x6 = 0x8 */
    set_register(0, 0x2);

    run_gen_instr(4);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x8);
    return 0;
}

//27
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

//28
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

//29
static int test_add_imm_nr_fl_1() {
  set_register(0, 0x0);

  struct dp_instr *gen_instruction = gen_instr(4);
  gen_instruction->op2 = 0x0;
  dp_exec((void*) gen_instruction);

  uint32_t reg_content = get_register(1);
  mu_assert(reg_content == 0x0 && get_zflag == 1 && get_nflag == 0);
  return 0;
}

//30
static int test_add_imm_nr_fl_2() {
  /* Overflow with result 0x0 */
  set_register(0, 0xFFFFFFFF);

  struct dp_instr *gen_instruction = gen_instr(4);
  gen_instruction->op2 = 0x1;
  dp_exec((void*) gen_instruction);

  uint32_t reg_content = get_register(1);
  mu_assert(reg_content == 0x0 && get_zflag == 1 && get_nflag == 0 && get_cflag == 1);
  return 0;
}

// TST TESTS
//31
static int test_tst_imm_nr() {
    /* 0x8 AND 0x6 = 0x4 -- RESULT NOT WRITTEN */
    set_register(0, 0x8);
    set_register(1, 0xF);

    run_gen_instr(5);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xF);
    return 0;
}

//32
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
//33
static int test_teq_imm_nr() {
    /* 0xC TEQ 0x6 = 0XA -- RESULT NOT WRITTEN */
    set_register(0, 0xC);
    set_register(1, 0xF);

    run_gen_instr(6);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xF);
    return 0;
}

//34
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
//35
static int test_cmp_imm_nr() {
    /* 0xA CMP 0x6 = 0x4 -- RESULT NOT WRITTEN */
    set_register(0, 10);
    set_register(1, 0xF);

    run_gen_instr(7);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xF);
    return 0;
}

//36
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
//37
static int test_orr_imm_nr() {
    /* 0xC OR 0x6 = 0xE */
    set_register(0, 0xC);

    run_gen_instr(8);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0xE);
    return 0;
}

//38
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
//39
static int test_mov_imm_nr() {
    /* 0x0 MOV 0x6 = 0x6 */
    set_register(0, 0x0);

    run_gen_instr(9);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x6);
    return 0;
}

//40
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

//41
static int test_mov_imm_nr_fl_1() {
    /* 0x0 MOV 0x6 = 0x6 */
    set_register(0, 0x0);

    run_gen_instr(9);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x6 && get_cflag == 0 && get_nflag == 0 && get_zflag == 0);
    return 0;
}

//42
static int test_mov_imm_nr_fl_2() {
    /* 0x0 MOV 0x6 = 0x6 */
    set_register(0, 0x0);

    struct dp_instr *gen_instruction = gen_instr(9);
    gen_instruction->op2 = 0x0;
    dp_exec((void*) gen_instruction);

    uint32_t reg_content = get_register(1);
    mu_assert(reg_content == 0x0 && get_cflag == 0 && get_nflag == 0 && get_zflag == 1);
    return 0;
}

// SHIFT TESTS WITH FLAGS
//43
static int test_shift_fl_orr() {
  /* Generic shift checking with carry flags*/
  set_register(0, 0xFFFFFFF0);
  set_register(3, 0x4);
  set_register(2, 0x000000F0);

  struct dp_instr *gen_instruction = gen_instr(8);
  gen_instruction->op2 = 0x332;
  gen_instruction->imm_op = 0;
  dp_exec((void*) gen_instruction);

  uint32_t reg_content = get_register(1);
  mu_assert(reg_content == 0xFFFFFFFF && get_cflag == 0 && get_nflag == 1 && get_zflag == 0);

  gen_instruction->op2 = 0x352;
  dp_exec((void*) gen_instruction);
  reg_content = get_register(1);
  mu_assert(reg_content == 0xFFFFFFFF && get_cflag == 0 && get_nflag == 1 && get_zflag == 0);

  gen_instruction->op2 = 0x372;
  dp_exec((void*) gen_instruction);
  reg_content = get_register(1);
  mu_assert(reg_content == 0xFFFFFFFF && get_cflag == 0 && get_nflag == 1 && get_zflag == 0);

  gen_instruction->op2 = 0x312;
  set_register(0, 0xFFFFFF0F);
  set_register(2, 0x00000078);
  set_register(3, 0x1);
  dp_exec((void*) gen_instruction);
  reg_content = get_register(1);
  mu_assert(reg_content == 0xFFFFFFFF && get_cflag == 0 && get_nflag == 1 && get_zflag == 0);
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
    mu_run_test(test_and_reg_nr);
    //5
    mu_run_test(test_and_reg_wr);
    //6
    mu_run_test(test_and_imm_nr_fl);
    //7
    mu_run_test(test_eor_imm_nr);
    //8
    mu_run_test(test_eor_imm_wr_1);
    //9
    mu_run_test(test_eor_imm_wr_2);
    //10
    mu_run_test(test_eor_reg_wr);
    //11
    mu_run_test(test_eor_reg_wr_fl);
    //12
    mu_run_test(test_sub_imm_nr_1);
    //13
    mu_run_test(test_sub_imm_nr_2);
    //14
    mu_run_test(test_sub_imm_wr_1);
    //15
    mu_run_test(test_sub_imm_wr_2);
    //16
    mu_run_test(test_sub_reg_nr);
    //17
    mu_run_test(test_sub_reg_wr_fl_1);
    //18
    mu_run_test(test_sub_reg_wr_fl_2);
    //19
    mu_run_test(test_sub_imm_nr_fl);
    //20
    mu_run_test(test_rsb_imm_nr_1);
    //21
    mu_run_test(test_rsb_imm_nr_2);
    //22
    mu_run_test(test_rsb_imm_wr_1);
    //23
    mu_run_test(test_rsb_imm_wr_2);
    //24
    mu_run_test(test_rsb_reg_nr_fl);
    //25
    mu_run_test(test_rsb_reg_wr_fl);
    //26
    mu_run_test(test_add_imm_nr);
    //27
    mu_run_test(test_add_imm_wr_1);
    //28
    mu_run_test(test_add_imm_wr_2);
    //29
    mu_run_test(test_add_imm_nr_fl_1);
    //30
    mu_run_test(test_add_imm_nr_fl_2);
    //31
    mu_run_test(test_tst_imm_nr);
    //32
    mu_run_test(test_tst_imm_wr);
    //33
    mu_run_test(test_teq_imm_nr);
    //34
    mu_run_test(test_teq_imm_wr);
    //35
    mu_run_test(test_cmp_imm_nr);
    //36
    mu_run_test(test_cmp_imm_wr);
    //37
    mu_run_test(test_orr_imm_nr);
    //38
    mu_run_test(test_orr_imm_wr);
    //39
    mu_run_test(test_mov_imm_nr);
    //40
    mu_run_test(test_mov_imm_wr);
    //41
    mu_run_test(test_mov_imm_nr_fl_1);
    //42
    mu_run_test(test_mov_imm_nr_fl_2);
    //43
    mu_run_test(test_shift_fl_orr);

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
