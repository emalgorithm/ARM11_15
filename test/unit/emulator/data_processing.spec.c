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

static struct dp_instr* gen_instr () {

    struct dp_instr *instruction;
    instruction = malloc(sizeof(struct dp_instr));
    
    instruction->imm_op = 1;
    instruction->op_code = 0;
    instruction->set_cond = 0;
    instruction->op1 = 0;
    instruction->dest = 1;
    instruction->op2 = 6;
    
    return instruction;
}

static void run_gen_instr () {
    struct dp_instr *gen_instruction = gen_instr();
    dp_exec((void*) gen_instruction); 
}

static int test_and_imm() {
    /* 0100 AND 0110
     * = 0100 */
    set_register(0, 8);
    
    run_gen_instr();
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==4);
    return 0;
}

static int test_eor_imm() {
    /* 1100 EOR 0110
     * = 1010 */
    set_register(0, 12);
    
    run_gen_instr();
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==10);
    return 0;
}

static int test_sub_imm() {
    /* 1010 SUB 0110
     * = 0100 */
    set_register(0, 10);
    
    run_gen_instr();
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==4);
    return 0;
}

static int test_rsb_imm() {
    /* 0011 RSB 0110
     * = 0011 */
    set_register(0, 3);
    
    run_gen_instr();
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==3);
    return 0;
}

static int test_add_imm() {
    /* 0010 ADD 0110
     * = 1000 */
    set_register(0, 2);
    
    run_gen_instr();
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==8);
    return 0;
}

static int test_tst_imm() {
    /* 0100 TST 0110
     * = 0100 -- RESULT NOT WRITTEN */
    set_register(0, 8);
    
    run_gen_instr();
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==0);
    return 0;
}

static int test_teq_imm() {
    /* 1100 TEQ 0110
     * = 1010 -- RESULT NOT WRITTEN */
    set_register(0, 12);
    
    run_gen_instr();
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==0);
    return 0;
}

static int test_cmp_imm() {
    /* 1010 CMP 0110
     * = 0100 -- RESULT NOT WRITTEN */
    set_register(0, 10);
    
    run_gen_instr();
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==0);
    return 0;
}

static int test_orr_imm() {
    /* 1100 OR 0110
     * = 1110 */
    set_register(0, 12);
    
    run_gen_instr();
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==14);
    return 0;
}

static int test_mov_imm() {
    /* 0000 EOR 0110
     * = 1010 */
    set_register(0, 0);
    
    run_gen_instr();
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==6);
    return 0;
}



static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_and_imm);
    mu_run_test(test_eor_imm);
    mu_run_test(test_sub_imm);
    mu_run_test(test_rsb_imm);
    mu_run_test(test_add_imm);
    mu_run_test(test_tst_imm);
    mu_run_test(test_teq_imm);
    mu_run_test(test_cmp_imm);
    mu_run_test(test_orr_imm);
    mu_run_test(test_mov_imm);

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


