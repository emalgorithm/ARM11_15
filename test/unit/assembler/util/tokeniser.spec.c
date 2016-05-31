#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../minunit.h"
#include "../../../../src/assembler/util/tokeniser.h"

#define SOURCE "../test/unit/assembler/util/asrc.s"

static char spec[] = "tokeniser";

int tests_run = 0;

void setup() {
    tokinit(SOURCE);
}

void tear_down() {
    tokdestroy();
}

static int test_next() {
    mu_assert(strcmp(toknext(), "mov") == 0);
    return 0;
}

static int test_nextreg() {
    toknext();

    mu_assert(tokreg() == 1);
    mu_assert(tokreg() == 2);

    return 0;
}

static int test_next_after_lbl() {
    toknext();

    mu_assert(strcmp(toknext(), "add") == 0);

    return 0;
}

static int test_label() {
    char *label = malloc(512);

    int address = toklabel(label);
    mu_assert(address == 4);
    mu_assert(strcmp(label, "l1") == 0);

    address = toklabel(label);
    mu_assert(address == 4);
    mu_assert(strcmp(label, "l2") == 0);

    address = toklabel(label);
    mu_assert(address == 8);
    mu_assert(strcmp(label, "l3") == 0);

    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_next);
    mu_run_test(test_nextreg);
    mu_run_test(test_next_after_lbl);
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
