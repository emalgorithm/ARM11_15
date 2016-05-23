#include <stdio.h>
#include "../minunit.h"
#include "../../../src/emulator/arm11.h"

static char spec[] = "arm11";
int tests_run = 0;

void setup() {
    initialize();
}

void tear_down() {}

static int test_initialize() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        mu_assert(get_byte(i) == 0);
    }

    for (int i = 0; i < NUM_OF_REGISTERS; i++) {
        mu_assert(get_register(i) == 0);
    }

    return 0;
}

static int test_get_and_set_byte() {

    for (uint32_t value = 0; value < (1 << 8); value++) {
        for (uint32_t i = 0; i < MEMORY_SIZE; i++) {
            set_byte(i, value);


            mu_assert(get_byte(i) == value);
        }
    }

    return 0;
}

static int test_get_and_set_word() {
    uint32_t address = 0x00000004;
    uint32_t big_endian_value = 0x01234567;
    uint32_t little_endian_value = 0x67452301;

    set_word(address, big_endian_value);

    mu_assert(get_byte(address) == 0x67);
    mu_assert(get_byte(address + 1) == 0x45);
    mu_assert(get_byte(address + 2) == 0x23);
    mu_assert(get_byte(address + 3) == 0x01);

    mu_assert(get_word(address) == big_endian_value);

    return 0;
}

static int test_get_and_set_register() {
    uint32_t index = 5;
    uint32_t value = 0x68FA6B55;

    set_register(index, value);

    mu_assert(get_register(index) == value);

    return 0;
}

// static int test_get_instr() {
//     uint32_t big_endian_instr = 0x01234567;
//     uint32_t little_endian_instr = 0x67452301;

//     for (uint32_t address = 0; address < MEMORY_SIZE; address += 4) {
//         set_word(address, big_endian_instr);
//         struct instruction *instr = get_instr(address);
//         union decoded_instr decoded = instr->decoded;

//         struct mul_instr = decoded.mul;

//         mu_assert(mul.rm == 0x7);
//         mu_assert(mul.rs == 0x5);
//         mu_assert(mul.rn == 0x4);
//         mu_assert(mul.rd == 0x3);
//         mu_assert(mul.s == 0x0);
//         mu_assert(mul.a == 0x1);
//         mu_assert(mul.cond == 0x0);

//     }

//     return 0;
// }

static int test_all() {
    printf("Running all tests for %s | ", spec);

     /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned.
     */


    mu_run_test(test_initialize);
    mu_run_test(test_get_and_set_byte);
    mu_run_test(test_get_and_set_word);
    mu_run_test(test_get_and_set_register);


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
