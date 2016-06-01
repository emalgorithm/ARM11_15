#include "writer_sec_pass.h"
#include "bwriter.h"

void write (union decoded_instr* instruction) {

    //printf("\n%x\n", *((uint32_t *) instruction));

    bwr_instr(instruction);

}
