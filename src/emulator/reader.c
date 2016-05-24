#include <stdio.h>
#include "reader.h"
#include "arm11.h"

void read_memory(char** argv) {
    uint8_t buffer[WORD_SIZE];
    FILE *input_file;
    input_file = fopen(argv[0], "rb");  // r for read, b for binary

    uint32_t address = 0;

    while(!feof(input_file)) {
        fread(buffer, sizeof(buffer), 1, input_file);

        uint32_t instr = buffer[WORD_SIZE - 1];

        for (int i = WORD_SIZE - 2; i >= 0; i--) {
            instr |= (((uint32_t)buffer[i]) << (BYTE * (WORD_SIZE - 1 - i)));
        }

        set_word(address, instr);

        address += 4;
    }
}
