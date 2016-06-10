/* Module  : bwriter
 *
 * Usage   : write a structure to a binary file
 *
 * Authors : Tencho Tenev
 */

#ifndef BWRITER_H
#define BWRITER_H

#include "../emulator/arm11.h"

/*
 * Function : bwr_init
 * -------------------
 * Setup binary writer. Must be called before writing.
 */
int bwr_init(const char *path);

/*
 * Function : write_instr
 * ----------------
 * Write the contents of a decoded instruction
 */
int bwr_instr(union decoded_instr *instr);

int bwr_data(uint32_t data);

/*
 * Function : bwr_destroy
 * ---------------------
 * Close the file stream and return non-zero if there were any errors
 */
int bwr_destroy(void);

#endif
