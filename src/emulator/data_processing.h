/* Module  : data_processing
 *
 * Usage   : A low-level module which processes instructions sent by the
 *           pipeline. Sets registers and CPSR flags accordingly.
 *
 * Authors : Alberto Spina
 */

#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

#include <stdio.h>
#include "arm11.h"

/*
 * Function : dp_exec
 * Usage    : dp_exec(union decoded_instr*)
 * ------------------------------
 * This procedure takes a pointer to a union decoded_instr* and executes the
 * command as if it were of type struct dp_instr*
 *
 * The Instruction must be of the following form:
 *
 * |----|--|-|------|-|--|--|--------|
 * |COND|00|I|OPCODE|S|RN|RD|OPERAND2|
 * |----|--|-|------|-|--|--|--------|
 *
 * Cond  : 4bits, checked before calling execution.
 * I     : 1bit, if set Op2 is an Immediate constant, shifted register otherwise
 * OpCode: Supported OpCodes:
 *         0000 - and : Rn AND Op2
 *         0001 - eor : Rn EOR Op2
 *         0010 - sub : Rn  -  Op2
 *         0011 - rsb : Op2 -  Rn
 *         0100 - add : Rn  +  Op2
 *         1000 - tst : AND - Not Written
 *         1001 - teq : EOR - Not Written
 *         1010 - cmp : SUB - Not Written
 *         1100 - orr : Rn  OR Op2
 *         1101 - mov : Op2
 * S     : 1bit, if set then CPSR flags should be updated
 * Rn    : 4bit, the first operand is the content of a register
 * Rd    : 4bit, destination register
 * Op2   : 12bits, second operand, if not immediate can be rotated as follows
 *
 * If Op2 is an Immediate Value, it is of the format:
 *
 * |------|---|
 * |Rotate|Imm|
 * |------|---|
 *
 * Rotate : 4bits, integer specifying a right rtation of Imm
 * Imm    : 8bits, immediate value, zero-extended to 32-bits
 *
 * If Op2 is NOT an Immediate Value (i.e. a Register) see -> "util/shift_reg.h"
 *
 * PRE  : The arm11 module is initialised.
 *        Registers content can be set and fetched.
 *
 * POST : Executes the given instruction as specified above.
 */

void dp_exec(union decoded_instr*);

#endif /*DATA_PROCESSING_H*/
