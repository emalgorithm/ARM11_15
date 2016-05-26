#include <stdlib.h>
#include <stdint.h>
#include "emulator/reader.h"
#include "emulator/printer.h"
#include "emulator/arm11.h"
#include "emulator/pipeline.h"

int main(int argc, char **argv) {
    // TODO
    initialize();

    read_memory(argv);

    emulate(0);

    print_registers();
    print_memory();

    destroy();

    return EXIT_SUCCESS;
}
