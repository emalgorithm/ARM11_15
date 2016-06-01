/* Module  : tokeniser
 *
 * Usage   : Extract tokens from Assembly source file
 *
 * Authors : Tencho Tenev
 */

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "tokeniser.h"
#include "hash_string.h"
#include "cond_map.h"
#include "shift_map.h"

#define OPEN_ERR "Tokeniser: Could not open file"
#define FILE_ERR "Tokeniser: File error"

// A line of assembler is at most 511 characters
#define BUFFER_SIZE 512
#define LABEL_COMMENT ";:"
#define DELIMITERS ", ["

#define PC 15

static FILE *src = NULL;

static char line[BUFFER_SIZE];

static char *token = NULL;

static uint32_t address = 0;

static char *next() {
    return strtok(NULL, DELIMITERS);
}

static bool lastchar(const char *str, char c) {
    return str && *str && str[strlen(str) - 1] == c;
}

static void clrnewline(char* str) {
    str[strcspn(str, "\r\n")] = 0;
}

static int _tokreg(char *regtoken, bool *positive) {
    assert(regtoken != NULL);

    // Check sign
    if (positive) {
        if (*regtoken == '-') {
            ++regtoken;
            *positive = false;
        } else {
            *positive = true;
        }
    }

    // Handle PC name
    if (strcmp(regtoken, "PC") == 0 || strcmp(regtoken, "PC]") == 0) {
        return PC;
    }

    // Skip [ and r
    while (!isdigit(*regtoken)) {
        ++regtoken;
    }

    return atoi(regtoken);
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
    return _tokreg(next(), NULL);
}

int toksignedreg(bool *positive) {
    return _tokreg(token, positive);
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
void tokop(enum operand_type *type) {
    assert(token != NULL);
    assert(type != NULL);

    token = next();

    if (!token || *token == '\n' || *token == ';') {
        // There is no operand
        *type = NONE;
    } else if (*token == '#') {
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
 * Return the shift code and set the type
 */
uint32_t tokshift(enum operand_type *type) {
    assert(type != NULL);

    token = next();

    char *shift = malloc(strlen(token) + 1);
    strcpy(shift, token);

    // Determine shift type
    tokop(type);

    return shift_map(hash(shift));
}

/*
 * Function : tokaddr
 * ------------------
 * Sets the type of addressing mode, operand type and base
 *
 * Addressing modes are MOV, PRE, and POST (use this to set the P bit or
 * translate the instruction as MOV)
 *
 * Operand types are ADDRESS, IMMEDIATE, NONE, and SHIFT_REG (use this to set
 * the I bit). In SHIFT_REG case examine the sign char for the shifted register
 * to set the UP bit if no sign + is used
 *
 * base_addr is a constant address in case the operand type is ADDRESS
 * or a register index in all other cases
 */
long tokaddr(enum addressing_mode *mode, enum operand_type *operand) {
    assert(src != NULL);

    long base_addr;
    token = next();

    if (*token == '=') {
        // Numeric constant address
        *operand = ADDRESS;

        ++token; // Skip = character
        base_addr = strtol(token, NULL, 0);

        *mode = (base_addr <= 0xff) ? MOV : PRE;
        return base_addr;

        /* MOV offset is retrieved with tokimm
         * PRE offset should be calculated and stored in memory
         *   offset = last address - (current address + 8)
         * The address to store is last address
         */
    }

    // Post or pre indexing - token is Rn] or Rn
    bool ispost = lastchar(token, ']');
    base_addr = _tokreg(token, NULL);

    tokop(operand);
    *mode = ispost && (*operand != NONE) ? POST : PRE;

    return base_addr;
}

/*
 * Function : tokcond
 * ------------------
 * Returns the hash condition part of an instruction token
 */
#define COND_SIZE 2
uint32_t tokcond(char *token) {
    int token_size = strlen(token);
    if (token_size <= COND_SIZE) {
        return 0; // should be hash of al
    }

    token += strlen(token) - COND_SIZE; // move pointer to last two characters

    return cond_map(hash(token));
}

int hastok() {
    return 1;
}

/*
 * Function : toklabel
 * -------------------
 * This function should only be used during the first pass for collecting
 * labels. Calling other tokeniser functions will corrupt the address counter.
 *
 * Returns the address of the next label in the source file and assigns its
 * name as the value of the provided pointer. The pointer be able to hold the
 * largest label possible to avoid memory errors. The returned address is the
 * address of the instruction directly after the label given that the first
 * instruction in the file is at address 0.
 */
uint32_t toklabel(char *label) {
    assert(src != NULL);
    assert(!feof(src));

    for (;;) {
        // Take next line
        if (fgets(line, BUFFER_SIZE, src) == NULL) {
            // End of file
            tokdestroy();
            return -1;
        }

        // Take first token of line
        token = strtok(line, DELIMITERS);

        // Skip empty lines and comments
        if (strchr(token, ';') || *token == '\n') {
            continue;
        }

        // Check for label
        clrnewline(token);
        if (lastchar(token, ':')) {
            // Remove colon from token
            token[strlen(token) - 1] = 0;
            // Copy to other pointer
            strcpy(label, token);
            return address;
        }

        // Must be an instruction so increase address
        address += 4;
    }
}
