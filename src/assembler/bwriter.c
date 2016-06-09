/* Module  : bwriter
 *
 * Usage   : Write a structure to a binary file
 *
 * Authors : Tencho Tenev
 */

#include "bwriter.h"
#include <stdio.h>
#include <stdint.h>

#define OPEN_ERR "BinWriter: Could not open file"
#define FILE_ERR "BinWriter: File error"

static FILE *dest = NULL;

/*
 * Function : bwr_init
 * -------------------
 * Setup binary writer. Must be called before writing.
 */
int bwr_init(const char *path) {
    dest = fopen(path, "wb");

    if (dest == NULL) {
        perror(OPEN_ERR);
        return 1;
    }

    return 0;
}

/*
 * Function : bwr_insrt
 * --------------------
 * Write the contents of a decoded instruction
 */
int bwr_instr(union decoded_instr *instr) {
    fwrite(instr, sizeof(uint32_t), 1, dest);
    return 0;
}

/*
 * Function : bwr_data
 * -------------------
 * Write the contents of an integer
 */
int bwr_data(uint32_t data) {
    fwrite(&data, sizeof(uint32_t), 1, dest);
    return 0;
}

/*
 * Function : bwr_destroy
 * ---------------------
 * Close the file stream and return non-zero if there were any errors
 */
int bwr_destroy(void) {
    if (ferror(dest)) {
        perror(FILE_ERR);

        fclose(dest);
        return 1;
    }

    fclose(dest);
    return 0;
}
