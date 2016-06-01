#include <stdlib.h>

#include "assembler/bwriter.h"
#include "assembler/second_pass.h"
#include "assembler/label_scanner.h"

int main(int argc, char **argv) {

    char* input_path = argv[1];

    char* output_path = argv[2];

    // TODO Run label_scanner
    scan_init(input_path);

    bwr_init(output_path);

    sec_pass_run(input_path);

    scan_terminate();
    bwr_destroy();

    return EXIT_SUCCESS;
}
