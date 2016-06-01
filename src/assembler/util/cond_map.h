/* Module  : cond_map
 *
 * Usage   : Map string hash to ARM11 condition code
 *
 * Authors : Tencho Tenev
 */

#ifndef CONDMAP_H
#define CONDMAP_H

#include <stdint.h>

/*
 * Function : cond_map
 * -------------------
 * Returns ARM11 condition code from string hash
 */
uint32_t cond_map(uint32_t hash);

#endif
