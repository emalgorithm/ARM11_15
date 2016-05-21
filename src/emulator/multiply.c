#include "multiply.h"

struct mul_instr* instr;
void mul_accumulate();
void mul_normal();

/* Pre: (*instruction).cond */
void mul_exec(void* instruction) {
    instr = (struct mul_instr*) instruction;

    if ((*instr).acc) {
        /* Multiply with accumulate */
    } else {
        /* Multiply without accumulate */
    }
}

void mul_accumulate() {
}

void mul_normal() {
}
