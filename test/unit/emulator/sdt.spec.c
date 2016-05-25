#include <stdio.h>
#include "../minunit.h"
#include "../../../src/emulator/sdt.h"
#include "../../../src/emulator/util/shift_reg.h"
#include "../../../src/emulator/arm11.h"

static char spec[] = "sdt";

int tests_run = 0;

void setup() {
    // arm11 state init
    initialize();
}

void tear_down() {}

/*
 * Test case for load_store = 1,
 * imm_off = 0, index_bit = 1,
 * up = 1
 */
static int test_sdt_exec_load() {
    union instruction instr;
    struct sdt_instr* sdt = &(instr.decoded.sdt);
    uint8_t rn_index = 0;
    uint8_t rd_index = 1;
    uint32_t rn_value = 10;
    set_register(rn_index, rn_value);
    uint32_t offset = 0x006;
    uint32_t mem_address = rn_value + offset;
    uint32_t mem_value = 341640;

    sdt->offset = offset;
    sdt->rd = rd_index;
    sdt->rn = rn_index;
    sdt->load_store = 1;
    sdt->up = 1;
    sdt->index_bit = 1;
    sdt->imm_off = 0;
    sdt->_id = 01;
    sdt->cond = 0000;

    set_word(mem_address, mem_value);

    sdt_exec(&(instr.decoded));

    mu_assert(get_register(rd_index) == get_word(mem_address));
    mu_assert(get_register(rn_index) == rn_value);

    return 0;
}

/*
 * Test case for load_store = 1,
 * imm_off = 1, index_bit = 1,
 * up = 1
 * We have a offset that is shifted
 * by 2 so we have a mutiple of 4
 */
static int test_sdt_exec_load_2() {
    union instruction instr;
    struct sdt_instr* sdt = &(instr.decoded.sdt);
    uint8_t rn_index = 0;
    uint8_t rd_index = 1;
    uint8_t shift_reg_index = 2;
    uint8_t shift_reg_value = 15;
    set_register(shift_reg_index, shift_reg_value);
    uint32_t rn_value = 16;
    set_register(rn_index, rn_value);
    uint32_t offset = 0x102;

    sdt->offset = offset;
    sdt->rd = rd_index;
    sdt->rn = rn_index;
    sdt->load_store = 1;
    sdt->up = 1;
    sdt->index_bit = 1;
    sdt->imm_off = 1;
    sdt->_id = 01;
    sdt->cond = 0000;

    offset = shift_reg(offset, 0);

    uint32_t mem_address = rn_value + offset;
    uint32_t mem_value = 341640;
    set_word(mem_address, mem_value);

    sdt_exec(&(instr.decoded));

    mu_assert(get_register(rd_index) == get_word(mem_address));
    mu_assert(get_register(rn_index) == rn_value);

    return 0;
}

/*
 * Test case for load_store = 1,
 * imm_off = 0, index_bit = 0,
 * up = 1
 */
static int test_sdt_exec_load_3() {
    union instruction instr;
    struct sdt_instr* sdt = &(instr.decoded.sdt);
    uint8_t rn_index = 0;
    uint8_t rd_index = 1;
    uint32_t rn_value = 8;
    set_register(rn_index, rn_value);
    uint32_t offset = 0x006;
    uint32_t mem_address = rn_value;
    uint32_t mem_value = 341640;

    sdt->offset = offset;
    sdt->rd = rd_index;
    sdt->rn = rn_index;
    sdt->load_store = 1;
    sdt->up = 1;
    sdt->index_bit = 0;
    sdt->imm_off = 0;
    sdt->_id = 01;
    sdt->cond = 0000;

    set_word(mem_address, mem_value);

    sdt_exec(&(instr.decoded));

    uint32_t new_rn_value = mem_address + offset;

    mu_assert(get_register(rd_index) == get_word(mem_address));
    mu_assert(get_register(rn_index) == new_rn_value);

    return 0;
}

/*
 * Test case for load_store = 1,
 * imm_off = 0, index_bit = 0,
 * up = 0
 */
static int test_sdt_exec_load_4() {
    union instruction instr;
    struct sdt_instr* sdt = &(instr.decoded.sdt);
    uint8_t rn_index = 0;
    uint8_t rd_index = 1;
    uint32_t rn_value = 8;
    set_register(rn_index, rn_value);
    uint32_t offset = 0x006;
    uint32_t mem_address = rn_value;
    uint32_t mem_value = 341640;

    sdt->offset = offset;
    sdt->rd = rd_index;
    sdt->rn = rn_index;
    sdt->load_store = 1;
    sdt->up = 0;
    sdt->index_bit = 0;
    sdt->imm_off = 0;
    sdt->_id = 01;
    sdt->cond = 0000;

    set_word(mem_address, mem_value);

    sdt_exec(&(instr.decoded));

    uint32_t new_rn_value = mem_address - offset;

    mu_assert(get_register(rd_index) == get_word(mem_address));
    mu_assert(get_register(rn_index) == new_rn_value);

    return 0;
}

/*
 * Test case for load_store = 0,
 * imm_off = 0, index_bit = 1,
 * up = 1
 */
static int test_sdt_exec_load_5() {
    union instruction instr;
    struct sdt_instr* sdt = &(instr.decoded.sdt);
    uint8_t rn_index = 0;
    uint8_t rd_index = 1;
    uint32_t rn_value = 10;
    uint32_t rd_value = 12430;
    set_register(rn_index, rn_value);
    set_register(rd_index, rd_value);
    uint32_t offset = 0x006;
    uint32_t mem_address = rn_value + offset;

    sdt->offset = offset;
    sdt->rd = rd_index;
    sdt->rn = rn_index;
    sdt->load_store = 0;
    sdt->up = 1;
    sdt->index_bit = 1;
    sdt->imm_off = 0;
    sdt->_id = 01;
    sdt->cond = 0000;

    sdt_exec(&(instr.decoded));

    mu_assert(get_word(mem_address) == rd_value);
    mu_assert(get_register(rn_index) == rn_value);

    return 0;
}


static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_sdt_exec_load);
    mu_run_test(test_sdt_exec_load_2);
    mu_run_test(test_sdt_exec_load_3);
    mu_run_test(test_sdt_exec_load_4);
    mu_run_test(test_sdt_exec_load_5);

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
