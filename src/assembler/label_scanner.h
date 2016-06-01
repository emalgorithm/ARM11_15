#ifndef LABEL_SCANNER_H
#define LABEL_SCANNER_H

#include <stdint.h>

/*
* This function must be called before using get_label_address.
* The function creates a map from labels to address in memory
* by scanning the file whose path is given as argument
*/
void scan_init(const char *path);

/*
* This function must be called when the scanner is no longer
* needed, since it frees the memory allocated by scan_init
*/
void scan_terminate();

/*
* This functions returns the address in memory of a given
* label passed as a parameter
*/
uint32_t get_label_address(const char *label);

#endif
