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

uint32_t shift_reg (uint32_t, uint32_t);

 uint32_t rot_right (uint32_t, uint32_t, uint32_t);

#endif
