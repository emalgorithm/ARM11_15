#include <stdio.h>
#include "../minunit.h"
#include "../../../src/disassembler/dis_exec.h"

static char spec[] = "dis_exec";

int tests_run = 0;

void setup() {
}

void tear_down() {
}

/*
 * Test case for load_store = 1,
 * imm_off = 0, index_bit = 1,
 * up = 1 */

/*static int test_second_pass_1() {

    sec_pass_run("../test/unit/assembler/test.txt");


    return 0;
}*/


static int test_second_pass_2() {

    char* input_path = "../test/unit/disassembler/test.o";
    //const char* output_path = "../test/unit/assembler/test_out.o";

    //bwr_init(output_path);

    disassemble_run(input_path);

    //bwr_destroy();

    return 0;
}


static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    //mu_run_test(test_second_pass_1);
    mu_run_test(test_second_pass_2);

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
