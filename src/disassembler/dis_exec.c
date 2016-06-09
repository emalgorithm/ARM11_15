#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#include "dis_exec.h"
#include "dis_dp.h"
#include "dis_mul.h"
#include "dis_sdt.h"
#include "dis_br.h"
#include "dis_andeq.h"
#include "dis_label.h"
#include "writer.h"
#include "../emulator/reader.h"
#include "../emulator/arm11.h"
#include "../emulator/pipeline.h"

static uint32_t pc;
static uint32_t max_pc;

static void init_pc();

static void (*dis_ptr)(char*, union decoded_instr*);

static void (*dis_decode (union instruction* instruction, bool* running))(char*, union decoded_instr*);

void disassemble_run (char *path) {

    union instruction* instruction;

    //generate_maps();
    dis_generate_dp_maps();
    //proc_sdt_init();

    //Initialises the Program Counter to 0
    init_pc();

    //Running is the condition for our loop
    bool* running = malloc(sizeof(bool));
    *running = true;

    while (*running) {
        dis_print_label(get_pc());

        instruction = get_instr(get_pc());

        dis_ptr = dis_decode(instruction, running);
        //If it is still running then execute the instruction
        dis_ptr(path, &instruction->decoded);

        // Update PC
        inc_pc();
    }
    free (running);

    //TODO: Destroy Maps
}

static void init_pc () {
    pc = 0;
}

void inc_pc () {
    pc += 4;
}

uint32_t get_pc () {
    return pc;
}

void set_max_pc (uint32_t val) {
    max_pc = val;
}

uint32_t get_max_pc () {
    return max_pc;
}

static void (*dis_decode (union instruction* instruction, bool* running))(char*, union decoded_instr*) {

    if (instruction->bin == 0) {
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
        exit(EXIT_FAILURE);
    }
    }
}
