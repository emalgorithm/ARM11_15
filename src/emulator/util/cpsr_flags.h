#ifndef CPSR_FLAGS_H
#define CPSR_FLAGS_H

/***************************************************************************
 * FILE NAME: cpsr_flags.h                                                 *
 *                                                                         *
 * PURPOSE: Helper functions for setting and retrieving CPRS FLAG          *
 *                                                                         *
 * DEVELOPMENT HISTORY:                                                    *
 *                                                                         *
 * Date          Author                Description of Changes              *
 * ----          ------                ----------------------              *
 * 24/05/2016    Alberto Spina         Initial version                     *
 ***************************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include "../arm11.h"
#include "binutils.h"

#define CPSR_FLAGS 16
#define N_FLAG 31
#define Z_FLAG 30
#define C_FLAG 29
#define V_FLAG 28

/*
 * Sets the bit at a given position to 1
 */
#define set_flag(pos) set_register(CPSR_FLAGS, get_register(CPSR_FLAGS) \
    | (1 << pos))

/*Sets the N-bit of the CPSR FLAGS*/
#define set_nflag set_flag(N_FLAG)

/*Sets the Z-bit of the CPSR FLAGS*/
#define set_zflag set_flag(Z_FLAG)

/*Sets the C-bit of the CPSR FLAGS*/
#define set_cflag set_flag(C_FLAG)

/*Sets the V-bit of the CPSR FLAGS*/
#define set_vflag set_flag(V_FLAG)

/*
 * Clears the flag at a given position to 0
 */
#define clr_flag(pos) set_register(CPSR_FLAGS, get_register(CPSR_FLAGS) \
    & ~(1 << pos))

/*Clears the N-bit of the CPSR FLAGS*/
#define clr_nflag clr_flag(N_FLAG)

/*Clears the Z-bit of the CPSR FLAGS*/
#define clr_zflag clr_flag(Z_FLAG)

/*Clears the C-bit of the CPSR FLAGS*/
#define clr_cflag clr_flag(C_FLAG)

/*Clears the V-bit of the CPSR FLAGS*/
#define clr_vflag clr_flag(V_FLAG)

/*
 * Returns the flag at a given position
 */
#define get_flag(pos) get_bit(get_register(CPSR_FLAGS), pos)

/*Returns the N-bit of the CPSR FLAGS*/
#define get_nflag get_flag(N_FLAG)

/*Returns the Z-bit of the CPSR FLAGS*/
#define get_zflag get_flag(Z_FLAG)

/*Returns the C-bit of the CPSR FLAGS*/
#define get_cflag get_flag(C_FLAG)

/*Returns the V-bit of the CPSR FLAGS*/
#define get_vflag get_flag(V_FLAG)

#endif
