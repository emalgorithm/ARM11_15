#include <stdio.h>
#include "minunit.h"
#include "../../src/emulator/data_processing.h"
#include "../../src/emulator/arm11.h"

static char spec[] = "data_processing";

int tests_run = 0;

/*TODO: Add tests for setconditions*/

static int test_and_imm() {
    set_register(0, 4);
    
    struct dp_instr *and_instruction;
    and_instruction = malloc(sizeof(struct dp_instr));
    
    and_instruction->imm_op = 1;
    and_instruction->op_code = 1;
    and_instruction->set_cond = 0;
    and_instruction->op1 = 0;
    and_instruction->dest = 1;
    and_instruction->op2 = 4;
    
    dp_exec((void*) and_instruction);
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==4);
    return 0;
}

static int test_add_imm() {
    set_register(0, 10);
    
    struct dp_instr *add_instruction;
    add_instruction = malloc(sizeof(struct dp_instr));
    
    add_instruction->imm_op = 1;
    add_instruction->op_code = 0;
    add_instruction->set_cond = 0;
    add_instruction->op1 = 0;
    add_instruction->dest = 1;
    add_instruction->op2 = 5;
    
    dp_exec((void*) add_instruction);
    
    uint32_t reg_content = get_register(1);
    mu_assert(reg_content==15);
    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_and_imm);
    mu_run_test(test_add_imm);

    return 0;
}

void setup() {

}

void tear_down() {

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


