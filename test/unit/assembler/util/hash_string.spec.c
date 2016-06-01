#include <stdio.h>
#include "../../minunit.h"
#include "../../../../src/assembler/util/hash_string.h"

static char spec[] = "hash_string";

int tests_run = 0;

void setup() {
}

void tear_down() {
}

static int test_hash() {
    uint32_t code = hash("and");
    mu_assert(0x00646e61 == code);

    return 0;
}

static int test_long_hash() {
    uint32_t code = hash("andaaaaaaaaaaaaaaaaaaaaaaa");
    mu_assert(0x61646e61 == code);

    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_hash);
    mu_run_test(test_long_hash);

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
