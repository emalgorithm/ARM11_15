#include <stdio.h>
#include "reader.h"
#include "arm11.h"

void read_memory(char** argv) {
    uint32_t *instr = malloc(sizeof(uint32_t));
    FILE *input_file = fopen(argv[1], "rb");  // r for read, b for binary

    uint32_t address = 0;

    for (;;) {
        fread(instr, sizeof(uint32_t), 1, input_file);

        if (feof(input_file)) {
            break;
        }

        set_word(address, *instr);

        address += 4;
    }

    free(instr);

    fclose(input_file);
}
