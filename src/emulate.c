#include <stdlib.h>
#include <stdint.h>
#include "reader.h"
#include "pipeline.h"
#include "printer.h"
#include "arm11.h"

#define MEMORY_SIZE (1 << 16)
#define NUM_OF_REGISTERS 17

int main(int argc, char **argv) {
    
    initialize();

    bin_to_mem(argv);

    emulate();

    print_registers();

    return EXIT_SUCCESS;
}
