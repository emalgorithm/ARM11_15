#include <stdio.h>
#include "../minunit.h"
#include "../../../src/disassembler/dis_exec.h"
#include "../../../src/disassembler/writer.h"
#include "../../../src/disassembler/dis_label.h"
#include "../../../src/assembler/second_pass.h"
#include "../../../src/assembler/bwriter.h"
#include "../../../src/assembler/label_scanner.h"
#include "../../../src/emulator/reader.h"
#include "../../../src/emulator/arm11.h"

static char spec[] = "dis_exec";

int tests_run = 0;

void setup() {
}

void tear_down() {
}

/*
 * Test case for load_store = 1,
 * imm_off = 0, index_bit = 1,
 * up = 1 */

/*static int test_second_pass_1() {

    sec_pass_run("../test/unit/assembler/test.txt");


    return 0;
}*/


static int test_second_pass_2() {

    const char* input_ass_path = "../test/unit/disassembler/input.s";
    const char* output_ass_path = "../test/unit/disassembler/bin.o";

    scan_init(input_ass_path);

    bwr_init(output_ass_path);

    sec_pass_run(input_ass_path);

    scan_terminate();
    bwr_destroy();

    char* input_path = "../test/unit/disassembler/bin.o";
    char* output_path = "../test/unit/disassembler/output.s";

    // Iinitialise memory
    initialize();

    //Builds char** needed for read_memory
    char** double_char_arr = malloc(2*sizeof(char*));
    double_char_arr[1] = input_path;

    //Reads and saves the memory at the input file
    read_memory(double_char_arr);

    dis_scan_init();

    file_init(output_path);

    disassemble_run(input_path);

    dis_scan_terminate();

    file_close();

    //bwr_destroy();

    return 0;
}


static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
     * The position of the failed one is returned and printed.
     * If all pass 0 is returned. */

    //mu_run_test(test_second_pass_1);
    mu_run_test(test_second_pass_2);

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
