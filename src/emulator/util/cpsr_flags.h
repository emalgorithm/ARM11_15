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

/*Sets the N-bit of the CPSR FLAGS*/
#define set_nflag set_register(CPSR_FLAGS, get_register(CPSR_FLAGS) \
    |= (1 << N_FLAG))

/*Sets the Z-bit of the CPSR FLAGS*/
#define set_zflag set_register(CPSR_FLAGS, get_register(CPSR_FLAGS) \
   |= (1 << Z_FLAG))

/*Sets the C-bit of the CPSR FLAGS*/
#define set_cflag set_register(CPSR_FLAGS, get_register(CPSR_FLAGS) \
    |= (1 << C_FLAG))

/*Sets the V-bit of the CPSR FLAGS*/
#define set_vflag set_register(CPSR_FLAGS, get_register(CPSR_FLAGS) \
    |= (1 << V_FLAG))

/*Clears the N-bit of the CPSR FLAGS*/
#define clr_nflag set_register(CPSR_FLAGS, get_register(CPSR_FLAGS) \
    &= (0 << N_FLAG))

/*Clears the Z-bit of the CPSR FLAGS*/
#define clr_zflag set_register(CPSR_FLAGS, get_register(CPSR_FLAGS) \
    &= (0 << Z_FLAG))

/*Clears the C-bit of the CPSR FLAGS*/
#define clr_cflag set_register(CPSR_FLAGS, get_register(CPSR_FLAGS) \
    &= (0 << C_FLAG))

/*Clears the V-bit of the CPSR FLAGS*/
#define clr_vflag set_register(CPSR_FLAGS, get_register(CPSR_FLAGS) \
    &= (0 << V_FLAG))

/*Returns the N-bit of the CPSR FLAGS*/
#define get_nflag get_bit(get_register(CPSR_FLAGS), N_FLAG)

/*Returns the Z-bit of the CPSR FLAGS*/
#define get_zflag get_bit(get_register(CPSR_FLAGS), Z_FLAG)

/*Returns the C-bit of the CPSR FLAGS*/
#define get_cflag get_bit(get_register(CPSR_FLAGS), C_FLAG)

/*Returns the V-bit of the CPSR FLAGS*/
#define get_vflag get_bit(get_register(CPSR_FLAGS), V_FLAG)
