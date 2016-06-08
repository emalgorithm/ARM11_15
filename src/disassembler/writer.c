#include "writer.h"

static FILE* file;

void file_init(char* path) {

    file = fopen (path, "w");

}

void file_write(char* str) {

    fprintf(file, "%s\n", str);

}

void file_close() {

    fclose(file);

}
