#include <stdio.h>
#include "../minunit.h"
#include "../../../src/util/binutils.h"

#define FAILED_MSG "Test failed"

int tests_run = 0;

static char * test_toggle_endian() {
    mu_assert(FAILED_MSG, toggle_endian(0xABCDEF12) == 0x12EFCDAB);
    return 0;
}

static char * test_getbits_0() {
    mu_assert(FAILED_MSG, getbits(0xA1B2C3D4, 0, 0) == 0);
    return 0;
}

static char * test_getbits() {
    mu_assert(FAILED_MSG, getbits(0xA1B2C3D4, 17, 4) == 0xB);
    return 0;
}

static char * test_get_word() {
    uint8_t mem[10] = {0x11, 0xAB, 0xBA, 0xF3, 0x08, 0xAB, 0xB5, 0xF4, 0xD3, 0xC2};
    mu_assert(FAILED_MSG, get_word(mem, 0) == 0xF3BAAB11);
    mu_assert(FAILED_MSG, get_word(mem, 3) == 0xB5AB08F3);
    mu_assert(FAILED_MSG, get_word(mem, 4) == 0xF4B5AB08);
    return 0;
}

static char * test_all() {
    mu_run_test(test_toggle_endian);
    mu_run_test(test_getbits_0);
    mu_run_test(test_getbits);
    mu_run_test(test_get_word);
    return 0;
}

int main(int argc, char **argv) {
    char *res = test_all();

    if (res != 0) {
        printf("%s\n", res);
    } else {
        printf("%s\n", "ALL TESTS PASS");
    }

    printf("%s %d\n", "NUMBER OF TESTS RUN", tests_run);

    return res != 0;
}
