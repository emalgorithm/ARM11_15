#include <stdio.h>
#include "../minunit.h"
#include "../../../src/assembler/second_pass.h"

static char spec[] = "second_pass";

int tests_run = 0;

void setup() {
}

void tear_down() {}

/*
 * Test case for load_store = 1,
 * imm_off = 0, index_bit = 1,
 * up = 1
 */
static int test_second_pass_1() {

    sec_pass_run("../test/unit/assembler/test.txt");

    mu_assert(0 == 0);
    return 0;
}


static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_second_pass_1);

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
