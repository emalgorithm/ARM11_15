#include <stdio.h>
#include "../minunit.h"
#include "../../../src/assembler/bwriter.h"
#include "../../../src/emulator/arm11.h"

static char spec[] = "bwriter";

int tests_run = 0;

void setup() {
    bwr_init("bin.out");
}

void tear_down() {
    bwr_destroy();
}

static int test_bwr() {
    union instruction instr;

    instr.bin = 0xfabc21ed;

    bwr_instr(&instr.decoded);

    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_bwr);

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
