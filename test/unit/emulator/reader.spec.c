#include <stdio.h>
#include "../minunit.h"
#include "../../../src/emulator/arm11.h"
#include "../../../src/emulator/reader.h"

static char spec[] = "reader";
int tests_run = 0;

void setup() {
    initialize();
}

void tear_down() {}

static int test_read_memory() {
    char* filename[] = {"executable_name",
                        "../test/unit/emulator/bne01"
                       };

    read_memory(filename);

    mu_assert(get_word(0) == 0x0110a0e3);
    mu_assert(get_word(4) == 0x0120a0e3);
    mu_assert(get_word(8) == 0x020051e1);
    mu_assert(get_word(12) == 0x0000001a);
    mu_assert(get_word(16) == 0x0330a0e3);
    mu_assert(get_word(20) == 0x0440a0e3);

    return 0;
}


static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
    * The position of the failed one is returned and printed.
    * If all pass 0 is returned.
    */


    mu_run_test(test_read_memory);

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
