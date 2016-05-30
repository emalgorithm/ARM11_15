/* Module  : tokeniser
 *
 * Usage   : Extract tokens from Assembly source file
 *
 * Authors : Tencho Tenev
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "tokeniser.h"

#define OPEN_ERR "Tokeniser: Could not open file"
#define FILE_ERR "Tokeniser: File error"

// A line of assembler is at most 511 characters
#define BUFFER_SIZE 512
#define LABEL_COMMENT ";:"
#define DELIMITERS ", "

static FILE *src = NULL;


char line[BUFFER_SIZE];

char *token = NULL;

static char *next() {
    return strtok(NULL, DELIMITERS);
}

/*
 * Function : initialise
 * ---------------------
 * This function must be called before using the rest of the interface
 *
 * path is a location of the file to be tokenised
 * returns 0 if the file was successfully opened for reading or non-zero on
 * error.
 */
int tokinit(const char *path) {
    src = fopen(path, "r");

    if (src == NULL) {
        perror(OPEN_ERR);
        return 1;
    }

    //fgets(line, BUFFER_SIZE, src);
    return 0;
}

/*
 * Function : tokdestroy
 * ---------------------
 * Close the file stream and return non-zero if there were any errors
 */
int tokdestroy() {
    if (ferror(src)) {
        perror(FILE_ERR);

        fclose(src);
        return 1;
    }

    fclose(src);
    return 0;
}

/*
 * Function : next
 * ---------------
 * Returns the token starting the next instruction skipping over anything from
 * the current position in the file.
 *
 * Instructions should not come on the same line as a label.
 * Ideally the file to be tokenised will have no labels and no comments - just
 * instructions on a new line. Maybe this normalisation should be done during
 * the first pass producing a temporary file for the second pass.
 *
 */
char *toknext() {
    assert(src != NULL);
    assert(!feof(src));

    do {
        // Take next line
        if (fgets(line, BUFFER_SIZE, src) == NULL) {
            // End of file
            tokdestroy();
            return NULL;
        }

        token = strtok(line, DELIMITERS);

    } while (strchr(token, ':') || strchr(token, ';') || *token == '\n');

    return token;
}

/*
 * Function : tokreg
 * -----------------------
 * Tries to tokenise and parse a register token.
 *
 * An assertion will fail if the next token is not a register.
 */
int tokreg() {
    assert(src != NULL);

    token = next();

    assert(*token == 'r');

    token++; // Skip r character

    return atoi(token);
}

/*
 * Function : tokop
 * ----------------------
 * Sets the type of the next operand. Other tokeniser functions can then be
 * safely used to get the remaining information.
 *
 * #<expression> numeric constant in decimal or hex (0x prefix)
 * rm, <shift>   shifted register
 *
 * An assertion will fail if the next token does not match an operand syntax.
 */
void tokop(enum operandType *type) {
    assert(token != NULL);
    assert(type != NULL);

    token = next();

    if (*token == '#') {
        *type = IMMEDIATE;
    } else {
        *type = SHIFT_REG;
    }
}

/*
 * Function : tokimm
 * ---------------------
 * Returns an immediate value from an expression (decimal or hex)
 *
 * Notice that if the numeric value returned by this function cannot be
 * represented by the instruction format an error should be produced by the
 * client code.
 */
long int tokimm() {
    assert(*token == '#');

    // Decimal or Hex
    ++token;

    return strtol(token, NULL, 0);
}

/*
 * Function : tokshift
 * -------------------
 * Return the name of a shift and set the type
 */
char *tokshift(enum operandType *type) {
    assert(token != NULL);
    assert(type != NULL);

    char *shift = malloc(strlen(token) + 1);
    strcpy(shift, token);

    // Determine shift type
    tokop(type);

    return shift;
}

/*
 * Function : tokaddr
 * ------------------
 * Sets the type of addressing mode
 */
void tokaddr(enum addressingMode *mode) {
}

/*
 * Function : tokcond
 * ------------------
 * Returns the condition part of an instruction token (al if no token)
 */
char *tokcond(char *token) {
    return NULL;
}

int hastok() {
    return 1;
}
