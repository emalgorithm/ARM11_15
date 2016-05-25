#include <stdio.h>
#include <stdint.h>
#include "../minunit.h"
#include "../../../src/emulator/util/shift_reg.h"
#include "../../../src/emulator/util/cpsr_flags.h"
#include "../../../src/emulator/arm11.h"

static char spec[] = "shift_reg";

int tests_run = 0;

void setup() {
    initialize();

    set_register(0, 0);

    clr_cflag;
}
void tear_down() {}

/*TESTS*/
//1
static int test_lsl_fl() {

    set_register(0, 0xF0000000);
    uint32_t instr = 0x100;
    uint32_t res = shift_reg(instr, 1);
    mu_assert(res == 0xC0000000 && get_cflag == 1);
    return 0;
}
//2
static int test_lsl_nfl() {

    set_register(0, 0xF0000000);
    uint32_t instr = 0x100;
    uint32_t res = shift_reg(instr, 0);
    mu_assert(res == 0xC0000000 && get_cflag == 0);
    return 0;
}
//3
static int test_lsr_fl() {

    set_register(0, 0xF);
    uint32_t instr = 0x120;
    uint32_t res = shift_reg(instr, 1);
    mu_assert(res == 0x3 && get_cflag == 1);
    return 0;
}
//4
static int test_lsr_nfl() {

    set_register(0, 0xF);
    uint32_t instr = 0x120;
    uint32_t res = shift_reg(instr, 0);
    mu_assert(res == 0x3 && get_cflag == 0);
    return 0;
}
//5
static int test_asr_fl_1() {

    set_register(0, 0x8000000F);
    uint32_t instr = 0x140;
    uint32_t res = shift_reg(instr, 1);
    mu_assert(res == 0xE0000003 && get_cflag == 1);
    return 0;
}
//6
static int test_asr_nfl_1() {

    set_register(0, 0x8000000F);
    uint32_t instr = 0x140;
    uint32_t res = shift_reg(instr, 0);
    mu_assert(res == 0xE0000003 && get_cflag == 0);
    return 0;
}
//7
static int test_asr_fl_2() {

    set_register(0, 0x4000000F);
    uint32_t instr = 0x140;
    uint32_t res = shift_reg(instr, 1);
    mu_assert(res == 0x10000003 && get_cflag == 1);
    return 0;
}
//8
static int test_asr_nfl_2() {

    set_register(0, 0x4000000F);
    uint32_t instr = 0x140;
    uint32_t res = shift_reg(instr, 0);
    mu_assert(res == 0x10000003 && get_cflag == 0);
    return 0;
}
//9
static int test_ror_fl() {

    set_register(0, 0x8000000F);
    uint32_t instr = 0x160;
    uint32_t res = shift_reg(instr, 1);
    mu_assert(res == 0xE0000003 && get_cflag == 1);
    return 0;
}
//10
static int test_ror_nfl() {

    set_register(0, 0x8000000F);
    uint32_t instr = 0x160;
    uint32_t res = shift_reg(instr, 0);
    mu_assert(res == 0xE0000003 && get_cflag == 0);
    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    //1
    mu_run_test(test_lsl_fl);
    //2
    mu_run_test(test_lsl_nfl);
    //3
    mu_run_test(test_lsr_fl);
    //4
    mu_run_test(test_lsr_nfl);
    //5
    mu_run_test(test_asr_fl_1);
    //6
    mu_run_test(test_asr_nfl_1);
    //7
    mu_run_test(test_asr_fl_2);
    //8
    mu_run_test(test_asr_nfl_2);
    //9
    mu_run_test(test_ror_fl);
    //10
    mu_run_test(test_ror_nfl);

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
