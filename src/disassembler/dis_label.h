#ifndef DIS_LABEL
#define DIS_LABEL

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/*
* This function must be called before using get_label.
* The function creates a map from addresses to labels in memory
* by scanning the file whose path is given as argument
*/
void dis_scan_init();

/*
* This function must be called when the scanner is no longer
* needed, since it frees the memory allocated by scan_init
*/
void dis_scan_terminate();

/*
* This functions given an empty string label and an address, assigns the value,
* if any, to the label for that address. Returns true if the label exists
* and has been assigned.
*/
void dis_print_label(uint32_t address);

char* dis_get_label(uint32_t address);

#endif
