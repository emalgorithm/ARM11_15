#include "dis_andeq.h"
#include "writer.h"

void dis_andeq_instr(char* path, union decoded_instr* instruction) {

    file_write("andeq r0, r0, r0\n");

}
