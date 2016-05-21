#include <stdio.h>
#include "../minunit.h"
#include "../../../src/emulator/util/binutils.h"

static char spec[] = "binutils";

int tests_run = 0;

static int test_toggle_endian() {
    mu_assert(toggle_endian(0xABCDEF12) == 0x12EFCDAB);
    return 0;
}

static int test_getbits_0() {
    mu_assert(getbits(0xA1B2C3D4, 0, 0) == 0);
    return 0;
}

static int test_getbits() {
    mu_assert(getbits(0xA1B2C3D4, 17, 4) == 0xB);
    return 0;
}

static int test_get_word() {
    uint8_t mem[10] = {0x11, 0xAB, 0xBA, 0xF3, 0x08, 0xAB, 0xB5, 0xF4, 0xD3, 0xC2};
    mu_assert(get_word(mem, 0) == 0xF3BAAB11);
    mu_assert(get_word(mem, 3) == 0xB5AB08F3);
    mu_assert(get_word(mem, 4) == 0xF4B5AB08);

    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_toggle_endian);
    mu_run_test(test_getbits_0);
    mu_run_test(test_getbits);
    mu_run_test(test_get_word);

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
