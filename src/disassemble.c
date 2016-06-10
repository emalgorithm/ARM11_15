#include <stdlib.h>

#include "disassembler/dis_exec.h"
#include "disassembler/writer.h"
#include "disassembler/dis_label.h"
#include "emulator/reader.h"
#include "emulator/arm11.h"

//First argument is the INPUT.o path
//Second argument is the OUTPUT.s path
int main(int argc, char **argv) {

    char* input_path = argv[1];

    char* output_path = argv[2];

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

    file_close();

    return EXIT_SUCCESS;
}
