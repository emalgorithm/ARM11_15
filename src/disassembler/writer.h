#ifndef WRITER
#define WRITER

#include <stdio.h>
#include <stdlib.h>

void file_init(char* path);

// MUST call file_init before file_write
void file_write(char* str);

void file_close();

#endif
