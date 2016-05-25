#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "sdt.h"
#include "util/shift_reg.h"

static bool check_out_of_boundary(uint32_t address) {
    if(!(address < MEMORY_SIZE)) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", address);
        return false;
    }

    return true;
}


static uint32_t get_scaled_mem_address(uint32_t mem_address, uint32_t offset, bool up) {
    uint32_t scaled_mem_address = mem_address;

    if (up) {
        scaled_mem_address += offset;
    } else {
        scaled_mem_address -= offset;
    }

    return scaled_mem_address;
}

static void load_from_mem(uint32_t dst_reg_index, uint32_t mem_address) {
    uint32_t value = get_word(mem_address);
    set_register(dst_reg_index, value);
}

static void store_into_mem(uint32_t src_reg_index, uint32_t mem_address) {
    uint32_t value = get_register(src_reg_index);
    set_word(mem_address, value);
}

void sdt_exec(union decoded_instr *decoded) {
    struct sdt_instr *sdt = &(decoded->sdt);

    assert(sdt->rd != PC_INDEX);

    uint32_t offset = sdt->offset;
    uint32_t mem_address = get_register(sdt->rn);

    if (sdt->imm_off) {
        offset = shift_reg(offset, 0);
    }

    /*
     * The flag is set: add/subtract offset before transferring the data
     */
    if (sdt->index_bit) {
        mem_address = get_scaled_mem_address(mem_address, offset, sdt->up);
    }

    if (!check_out_of_boundary(mem_address)) {
        return;
    }

    if (sdt->load_store) {
        load_from_mem(sdt->rd, mem_address);
    } else {
        store_into_mem(sdt->rd, mem_address);
    }

    /*
     * The flag is not set: add/subtract offset after transferring the data
     */
    if (!sdt->index_bit) {
        mem_address = get_scaled_mem_address(mem_address, offset, sdt->up);

        if (!check_out_of_boundary(mem_address)) {
            return;
        }

        set_register(sdt->rn, mem_address);
    }
}
