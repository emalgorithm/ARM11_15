#include <stdbool.h>

#include "dis_exec.h"
#include "../emulator/reader.h"
#include "../emulator/arm11.h"
#include "../emulator/pipeline.h"

static void dis_decode (union instruction* instruction);

void disassemble_run (char *path) {

    union instruction* instruction;

    //generate_maps();
    //generate_dp_maps();
    //proc_sdt_init();

    // Iinitialise tokeniser
    initialize();

    char** double_char_arr = malloc(2*sizeof(char*));
    double_char_arr[1] = path;

    read_memory(double_char_arr);

    uint32_t pc = 0;

    while (true) {
        instruction = get_instr(pc);

        dis_decode(instruction);

        // Update PC
        pc += 4;
    }
}

static void dis_decode (union instruction* instruction) {

}
