#ifndef IMPORTS
#define IMPORTS

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "util/tokeniser.h"
#include "../emulator/arm11.h"
#include "util/hashmap.h"
#include "util/func_hashmap.h"

void write (union decoded_instr*);

#endif
