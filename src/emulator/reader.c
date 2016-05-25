#include <stdio.h>
#include "reader.h"
#include "arm11.h"

void read_memory(char** argv) {
    uint8_t buffer[WORD_SIZE];
    FILE *input_file;
    input_file = fopen(argv[1], "rb");  // r for read, b for binary

    uint32_t address = 0;

    for (;;) {
        fread(buffer, sizeof(buffer), 1, input_file);

        if(feof(input_file)) {
            break;
        }

        uint32_t instr = buffer[0];

        for (int i = 1; i < WORD_SIZE; i++) {
            instr |= (((uint32_t)buffer[i]) << (BYTE * (i)));
        }

        set_word(address, instr);

        address += 4;
    }
}
