#include <stdlib.h>

#include "disassembler/dis_exec.h"
#include "disassembler/writer.h"

//First argument is the INPUT.o path
//Second argument is the OUTPUT.s path
int main(int argc, char **argv) {

    char* input_path = argv[1];

    char* output_path = argv[2];

    file_init(output_path);

    disassemble_run(input_path);

    file_close();

    return EXIT_SUCCESS;
}
