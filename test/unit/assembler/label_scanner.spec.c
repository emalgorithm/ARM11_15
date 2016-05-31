#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../minunit.h"
#include "../../../src/assembler/label_scanner.h"

#define SOURCE "../test/unit/assembler/util/asrc.s"

static char spec[] = "label_scanner";

int tests_run = 0;

void setup() {
    scan_init(SOURCE);
}

void tear_down() {
    scan_terminate();
}

static int test_label() {
    mu_assert(get_label_address("l1") == 4);
    mu_assert(get_label_address("l2") == 4);
    mu_assert(get_label_address("l3") == 8);
    mu_assert(get_label_address("l4") == 12);

    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_label);

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
