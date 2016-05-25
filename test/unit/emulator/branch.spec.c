#include <stdio.h>
#include "../minunit.h"
#include "../../../src/emulator/branch.h"
#include "../../../src/emulator/pipeline.h"
#include "../../../src/emulator/arm11.h"

static char spec[] = "branch";

int tests_run = 0;

void setup() {
    // arm11 state init
    initialize();

    em_reset();
}

void tear_down() {}

static int test_branch() {
    union instruction branch;

    em_set_pc(0);

    branch.bin = 0xea0000ff; // offset is 0xffff

    br_exec(&(branch.decoded));

    mu_assert(em_get_pc() == 0x3fc);

    return 0;
}

static int test_branch_1() {
    union instruction branch;
    uint32_t pc = 0x000001ff;
    uint32_t offset = 0x00000012;
    uint32_t transformed = 0x00000048;
    uint32_t branch_bin = 0xea000000;

    em_set_pc(pc);
    branch.bin = branch_bin + offset;

    br_exec(&(branch.decoded));

    mu_assert(em_get_pc() == pc + transformed);

    return 0;
}

static int test_branch_neg() {
    union instruction branch;
    uint32_t pc = 0x0000f1ff;
    uint32_t offset = 0x00ffffff;
    int32_t transformed = 0xfffffffc;
    uint32_t branch_bin = 0xea000000;

    em_set_pc(pc);
    branch.bin = branch_bin | offset;

    br_exec(&(branch.decoded));

    mu_assert(em_get_pc() == pc + transformed);

    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    mu_run_test(test_branch);
    mu_run_test(test_branch_1);
    mu_run_test(test_branch_neg);

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
