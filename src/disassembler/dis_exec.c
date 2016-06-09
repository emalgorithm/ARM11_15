#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#include "dis_exec.h"
#include "dis_dp.h"
#include "dis_mul.h"
#include "dis_sdt.h"
#include "dis_br.h"
#include "dis_andeq.h"
#include "writer.h"
#include "../emulator/reader.h"
#include "../emulator/arm11.h"
#include "../emulator/pipeline.h"

static void (*dis_ptr)(char*, union decoded_instr*);

static void (*dis_decode (union instruction* instruction, bool* running))(char*, union decoded_instr*);

void disassemble_run (char *path) {

    union instruction* instruction;

    //generate_maps();
    dis_generate_dp_maps();
    //proc_sdt_init();

    // Iinitialise memory
    initialize();

    //Builds char** needed for read_memory
    char** double_char_arr = malloc(2*sizeof(char*));
    double_char_arr[1] = path;

    //Reads and saves the memory at the input file
    read_memory(double_char_arr);

    //Initialises the Program Counter to 0
    uint32_t pc = 0;

    //Running is the condition for our loop
    bool* running = malloc(sizeof(bool));
    *running = true;

    while (*running) {
        instruction = get_instr(pc);

        dis_ptr = dis_decode(instruction, running);
        //If it is still running then execute the instruction
        dis_ptr(path, &instruction->decoded);

        // Update PC
        pc += 4;
    }
    free (running);

    // Destroy Maps
}

static void (*dis_decode (union instruction* instruction, bool* running))(char*, union decoded_instr*) {

    if(instruction->bin == 0) {
        *running = false;
        //printf("ZERO\n");
        return dis_andeq_instr;
    }
    switch (instruction->decoded.dp._id) {

    case DP_MULT_ID: {
        if (instruction->decoded.dp.imm_op == 1) {
          return dis_dp_instr;
        }

        if (!instruction->decoded.mul._mul4) {
          return dis_dp_instr;
        }

        if (!instruction->decoded.mul._mul7) {
          return dis_dp_instr;
        }

        return dis_mul_instr;
    }

    case SDT_ID: {
        return dis_sdt_instr;
    }

    case BRANCH_ID: {
        return dis_br_instr;
    }

    default: {
        assert(false); // Invalid instruction
    }
    }
}
