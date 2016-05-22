#include <stdio.h>
#include "../minunit.h"
#include "../../../src/emulator/arm11.h"

static char spec[] = "arm11";
int tests_run = 0;

void setup() {}
void tear_down() {}

static int test_initialize() {


    return 0;

}

static int test_get_and_set_word() {
    uint32_t address = 0x0000A0B3;
    uint32_t value = 432;

    set_word(address, value);

    mu_assert(get_word(address) == value);

    return 0;

}

static int test_get_and_set_register() {
    uint32_t index = 5;
    uint32_t value = 0x68FA6B55;

    set_register(index, value);

    mu_assert(get_register(index) == value);

    return 0;

}

static int test_all() {
    printf("Running all tests for %s | ", spec);

     /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned.
     */

    mu_run_test(test_get_and_set_word);
    mu_run_test(test_get_and_set_register);


    return 0;
}

int main(int argc, char **argv) {
    initialize();

    int res = test_all();

    if (res != 0) {
        printf("TEST #%d FAILED.\n", res);
    } else {
        printf("%d passed\n", tests_run);
    }

    return res != 0;
}
