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

static int test_get_instr_dp() {
    uint32_t big_endian_instr = 0x01234567;

    for (uint32_t address = 0; address < MEMORY_SIZE; address += 4) {
        set_word(address, big_endian_instr);
        union instruction *instr = get_instr(address);
        union decoded_instr decoded = instr->decoded;


        struct dp_instr dp = decoded.dp;

        mu_assert(dp.op2 == 0x567);
        mu_assert(dp.rd == 0x4);
        mu_assert(dp.rn == 0x3);
        mu_assert(dp.set_cond == 0x0);
        mu_assert(dp.op_code == 0x9);
        mu_assert(dp.imm_op == 0x0);
        mu_assert(dp._id == 0x0);
        mu_assert(dp.cond == 0x0);
    }

    return 0;
}

static int test_get_instr_mul() {
    uint32_t big_endian_instr = 0x10245697;

    for (uint32_t address = 0; address < MEMORY_SIZE; address += 4) {
        set_word(address, big_endian_instr);
        union instruction *instr = get_instr(address);
        union decoded_instr decoded = instr->decoded;


        struct mul_instr mul = decoded.mul;

        mu_assert(mul.rm == 0x7);
        mu_assert(mul._mul4 == 0x1);
        mu_assert(mul._mul7 == 0x1);
        mu_assert(mul.rs == 0x6);
        mu_assert(mul.rn == 0x5);
        mu_assert(mul.rd == 0x4);
        mu_assert(mul.set_cond == 0x0);
        mu_assert(mul.acc == 0x1);
        mu_assert(mul.cond == 0x1);
    }

    return 0;
}

static int test_get_instr_sdt() {
    uint32_t big_endian_instr = 0x05834567;

    for (uint32_t address = 0; address < MEMORY_SIZE; address += 4) {
        set_word(address, big_endian_instr);
        union instruction *instr = get_instr(address);
        union decoded_instr decoded = instr->decoded;


        struct sdt_instr sdt = decoded.sdt;

        mu_assert(sdt.offset == 0x567);
        mu_assert(sdt.rd == 0x4);
        mu_assert(sdt.rn == 0x3);
        mu_assert(sdt.load_store == 0x0);
        mu_assert(sdt.up == 0x1);
        mu_assert(sdt.index_bit == 0x1);
        mu_assert(sdt.imm_off == 0x0);
        mu_assert(sdt._id == 0x1);
        mu_assert(sdt.cond == 0x0);
    }

    return 0;
}



static int test_get_instr_br() {
    uint32_t big_endian_instr = 0x0A123456;

    for (uint32_t address = 0; address < MEMORY_SIZE; address += 4) {
        set_word(address, big_endian_instr);
        union instruction *instr = get_instr(address);
        union decoded_instr decoded = instr->decoded;


        struct br_instr mul = decoded.br;

        mu_assert(mul.offset == 0x123456);
        mu_assert(mul._id == 0x2);
        mu_assert(mul.cond == 0x0);
    }

    return 0;
}

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
    mu_run_test(test_get_instr_dp);
    mu_run_test(test_get_instr_mul);
    mu_run_test(test_get_instr_sdt);
    mu_run_test(test_get_instr_br);

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
