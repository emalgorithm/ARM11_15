#include <stdio.h>
#include <stdint.h>
#include "arm11.h"
#include "printer.h"
#include "util/binutils.h"

#define PC_LBL "PC"
#define CPSR_LBL "CPSR"
#define REG_TITLE "Registers:"
#define MEM_TITLE "Non-zero memory:"

static void print_register(uint32_t index);
static void print_reg_label(uint32_t index, char *label);
static void print_word(uint32_t address);

void print_registers(void) {
    printf("%s\n", REG_TITLE);

    for (uint32_t i = 0; i < NUM_OF_REGISTERS - 4; i++) {
      print_register(i);
    }

    print_reg_label(PC_INDEX, PC_LBL);
    print_reg_label(CPSR_INDEX, CPSR_LBL);
}

static void print_register(uint32_t index) {
    uint32_t value = get_register(index);
    printf("$%-3d: %10d (0x%08x)\n", index, value, value);

}

static void print_reg_label(uint32_t index, char *label) {
    uint32_t value = get_register(index);
    printf("%-4s: %10d (0x%08x)\n", label, value, value);

}

void print_memory(void) {
    printf("%s\n", MEM_TITLE);
    for (uint32_t i = 0; i < MEMORY_SIZE; i += 4) {
        print_word(i);
    }
}

static void print_word(uint32_t address) {
    uint32_t value = toggle_endian(get_word(address));
    if(value) {
        printf("0x%08x: 0x%08x\n", address, value);
    }
}
