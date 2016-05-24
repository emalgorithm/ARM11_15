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

void dp_exec(union decoded_instr*);

#endif /*DATA_PROCESSING_H*/
