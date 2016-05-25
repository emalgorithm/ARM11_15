#include <stdio.h>
#include <stdint.h>
#include "arm11.h"
#include "printer.h"
#include "util/binutils.h"

static void print_register(uint32_t index);
static void print_word(uint32_t address);

void print_registers(void) {
    for (uint32_t i = 0; i < NUM_OF_REGISTERS - 2; i++) {
      print_register(i);
    }

    print_register(PC_INDEX);
    print_register(CPSR_INDEX);
}

static void print_register(uint32_t index) {
    uint32_t value = get_register(index);
    printf("$%d: %d %x\n", index, value, value);

}

void print_memory(void) {
    for (uint32_t i = 0; i < MEMORY_SIZE; i += 4) {
        print_word(i);
    }
}

static void print_word(uint32_t address) {
    uint32_t value = get_word(address);
    if(value) {
        printf("%x: %x\n", address, value);
    }
}
