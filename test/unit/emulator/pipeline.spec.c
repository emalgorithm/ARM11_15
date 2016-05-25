#include <stdio.h>
#include "../minunit.h"
#include "../../../src/emulator/pipeline.h"
#include "../../../src/emulator/arm11.h"

static char spec[] = "pipeline";

int tests_run = 0;

void setup() {
    // arm11 state init
    initialize();

    em_reset();
}

void tear_down() {}

static int test_status_def() {
    mu_assert(em_get_status() == initial);

    return 0;
}

static int test_emulate_empty_mem() {
    mu_assert(emulate(0) == 0);
    mu_assert(em_get_status() == terminated);

    return 0;
}

static int test_emulate_conseq_run() {
    mu_assert(emulate(0) == 0);
    mu_assert(em_get_status() == terminated);

    mu_assert(emulate(0) == 1);
    mu_assert(emulate(0) == 1);
    mu_assert(emulate(0) == 1);

    return 0;
}

static int test_reset() {

    mu_assert(emulate(0) == 0);
    mu_assert(em_get_status() == terminated);

    em_reset();

    mu_assert(em_get_status() == initial);

    return 0;
}

static int test_set_pc() {
    em_set_pc(0x0000ff00);

    mu_assert(em_get_pc() == (0x0000ff00));

    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_status_def);
    mu_run_test(test_emulate_empty_mem);
    mu_run_test(test_emulate_conseq_run);
    mu_run_test(test_reset);
    mu_run_test(test_set_pc);

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
