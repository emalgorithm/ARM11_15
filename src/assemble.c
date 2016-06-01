#include <stdlib.h>

#include "assembler/bwriter.h"
#include "assembler/second_pass.h"

int main(int argc, char **argv) {

    char* input_path = argv[1];

    char* output_path = argv[2];

    // TODO Run label_scanner

    bwr_init(output_path);

    sec_pass_run(input_path);

    bwr_destroy();

    return EXIT_SUCCESS;
}
