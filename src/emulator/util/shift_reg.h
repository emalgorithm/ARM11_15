#ifndef SHIFT_REG_H
#define SHIFT_REG_H

/***************************************************************************
 * FILE NAME: shift_reg.h                                                  *
 *                                                                         *
 * PURPOSE: Helper functions for shifting                                  *
 *                                                                         *
 * DEVELOPMENT HISTORY:                                                    *
 *                                                                         *
 * Date          Author                Description of Changes              *
 * ----          ------                ----------------------              *
 * 24/05/2016    Alberto Spina         Initial version                     *
 ***************************************************************************/

#include <stdint.h>
#include "../arm11.h"

/*
 * Function : shift_reg
 * Usage    : dp_exec(0x0, 0x0)
 * ------------------------------
 * This procedure takes a uint32_t instruction that will be shifted according
 * to its Shift Value. It also takes a uint32_t set_cond that specifies whether
 * the given instruction should set flags or not
 *
 * The Instruction must be of the following form:
 *
 * |-----|--|
 * |Shift|Rm|
 * |-----|--|
 *
 * Shift : 8bits, specified below
 * Rm    : 4bits, shift operations are applied to the value held in Rm
 *
 * Shift by a Constant Amount:
 *
 * |-------|---------|-|
 * |Integer|ShiftType|0|
 * |-------|---------|-|
 *
 * Int     : 5bits, specifies the Shift Amount
 * ShiftTy : 2bits, as below
 *
 * Shift by a Register
 *
 * |--|-|---------|-|
 * |Rs|0|ShiftType|1|
 * |--|-|---------|-|
 *
 * Rs      : 4bits, specifies the register containing the Shift Amount
 * ShiftTy : 2bits, as below
 *
 * SHiftTy : Supported Shift Types
 *         00 - lsl : Logical Shift Left
 *         01 - lsr : Logical Shift Right
 *         10 - asr : Arithmetic Shift Right
 *         11 - ror : Rotate Right
 *
 * PRE  : Not NULL instruction
 *
 * POST : Returns the given shifted instruction as specified above.
 */

// shift_reg(uint32_t instruction, uint32_t set_cond)
uint32_t shift_reg (uint32_t, uint32_t);

// uint32_t rot_right(uint32_t amount, uint32_t value, uint32_t set_cond)
uint32_t rot_right (uint32_t, uint32_t, uint32_t);

#endif
