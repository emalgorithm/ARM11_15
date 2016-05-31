/* Module  : tokeniser
 *
 * Usage   : Extract tokens from Assembly source file
 *
 * Authors : Tencho Tenev
 */

#include <stdint.h>

/*
 * Function : tokinit
 * ---------------------
 * This function must be called before using the rest of the interface
 *
 * path is a location of the file to be tokenised
 * returns 0 if the file was successfully opened for reading or non-zero on
 * error.
 */
int tokinit(const char *path);

/*
 * Function : tokdestroy
 * ---------------------
 * Close the file stream and return non-zero if there were any errors
 */
int tokdestroy();

/*
 * Function : next
 * ---------------
 * Returns the token starting the next instruction skipping over anything from
 * the current position in the file
 */
char *toknext();

/*
 * Function : tokreg
 * -----------------------
 * Tries to tokenise and parse a register token.
 *
 * An assertion will fail if the next token is not a register.
 */
int tokreg();


enum operandType {
    IMMEDIATE,
    SHIFT_REG
};

/*
 * Function : tokop
 * ----------------------
 * Returns the type of the next operand. Other tokeniser functions can then be
 * safely used to get the remaining information.
 *
 * An assertion will fail if the next token does not match an operand syntax.
 */
void tokop(enum operandType *type);

/*
 * Function : tokimm
 * ---------------------
 * Returns an immediate value from an expression (decimal or hex)
 *
 * Notice that if the numeric value returned by this function cannot be
 * represented by the instruction format an error should be produced by the
 * client code.
 */
long int tokimm();

/*
 * Function : tokshift
 * -------------------
 * Return the name of a shift and set the type
 */
char *tokshift(enum operandType * type);

enum addressingMode {
    MOV,
    OFFSET,
    PRE,
    POST
};

/*
 * Function : tokaddr
 * ------------------
 * Sets the type of addressing mode
 */
void tokaddr(enum addressingMode * mode);

/*
 * Function : tokcond
 * ------------------
 * Returns the condition part of an instruction token (al if no token)
 */
char *tokcond(char *token);

/*
 * Function : tokcond
 * ------------------
 * Returns true if the file has tokens left.
 */
int hastok();

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
uint32_t toklabel(char *label);
