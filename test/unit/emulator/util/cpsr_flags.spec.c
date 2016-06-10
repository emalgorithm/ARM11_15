#include <stdio.h>
#include <stdint.h>
#include "../../minunit.h"
#include "../../../../src/emulator/util/cpsr_flags.h"
#include "../../../../src/emulator/arm11.h"

static char spec[] = "cpsr_flags";

int tests_run = 0;

void setup() {
    initialize();
}
void tear_down() {}

/*SET TESTS*/
//1
static int test_nflag_set() {
    set_nflag;
    mu_assert(get_nflag == 1);
    return 0;
}

//2
static int test_zflag_set() {
    set_zflag;
    mu_assert(get_zflag == 1);
    return 0;
}

//3
static int test_cflag_set() {
    set_cflag;
    mu_assert(get_cflag == 1);
    return 0;
}

//4
static int test_vflag_set() {
    set_vflag;
    mu_assert(get_vflag == 1);
    return 0;
}

/*CLEAR TESTS*/
//5
static int test_nflag_clr() {
    clr_nflag;
    mu_assert(get_nflag == 0);
    return 0;
}

//6
static int test_zflag_clr() {
    clr_zflag;
    mu_assert(get_zflag == 0);
    return 0;
}

//7
static int test_cflag_clr() {
    clr_cflag;
    mu_assert(get_cflag == 0);
    return 0;
}

//8
static int test_vflag_clr() {
    clr_vflag;
    mu_assert(get_vflag == 0);
    return 0;
}

/*GENERAL CASE*/
//9
static int test_mix() {
    set_nflag;
    clr_zflag;
    set_cflag;
    clr_vflag;
    mu_assert(get_nflag == 1 && get_zflag == 0 && get_cflag == 1 && get_vflag == 0);
    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    //1
    mu_run_test(test_nflag_set);
    //2
    mu_run_test(test_zflag_set);
    //3
    mu_run_test(test_cflag_set);
    //4
    mu_run_test(test_vflag_set);
    //5
    mu_run_test(test_nflag_clr);
    //6
    mu_run_test(test_zflag_clr);
    //7
    mu_run_test(test_cflag_clr);
    //8
    mu_run_test(test_vflag_clr);
    //9
    mu_run_test(test_mix);

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
