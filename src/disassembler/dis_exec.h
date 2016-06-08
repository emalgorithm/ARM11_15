#ifndef DIS_EXEC
#define DIS_EXEC

#include <stdint.h>

struct dp_dis {
    char* instr;
    char* rd;
    char* rn;
    char* op2;
};

struct mul_dis {
    char* instr;
    char* rd;
    char* rn;
    char* rs;
    char* rm;
};

struct sdt_dis {
    char* instr;
    char* rd;
    char* rn;
    char* off;
};

void disassemble_run (char *path);

#endif
