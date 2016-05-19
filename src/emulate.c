#include <stdlib.h>
#include <stdint.h>
#include "reader.h"
#include "pipeline.h"
#include "printer.h"

#define MEMORY_SIZE 1 << 16
#define NUM_OF_REGISTERS 17

struct state {
    uint8_t memory[MEMORY_SIZE];  
    uint32_t registers[NUM_OF_REGISTERS];
};

struct state arm11; 

int main(int argc, char **argv) {
    
    clear_memory();

    bin_to_mem(argv);

    emulate();

    print_registers();

    return EXIT_SUCCESS;
}
