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

static char * test_all() {
    mu_run_test(test_toggle_endian);
    mu_run_test(test_getbits_0);
    mu_run_test(test_getbits);
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
