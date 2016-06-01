#include <stdio.h>
#include <stdint.h>
#include "../minunit.h"
#include "../../../src/util/binutils.h"

static char spec[] = "binutils";

int tests_run = 0;

void setup() {}
void tear_down() {}

static int test_toggle_endian() {
    mu_assert(toggle_endian(0xABCDEF12) == 0x12EFCDAB);
    return 0;
}

static int test_get_bits_0() {
    mu_assert(get_bits(0xA1B2C3D4, 0, 0) == 0);
    return 0;
}

static int test_get_bits() {
    mu_assert(get_bits(0xA1B2C3D4, 17, 4) == 0xB);
    return 0;
}

static int test_set_byte_at() {
    uint32_t source = 0xfa12fb34;
    set_byte_at(&source, 0, 0xff);

    mu_assert(source == 0xfa12fbff);

    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_toggle_endian);
    mu_run_test(test_get_bits_0);
    mu_run_test(test_get_bits);
    mu_run_test(test_set_byte_at);

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
