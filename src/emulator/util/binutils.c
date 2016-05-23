/***************************************************************************
 * FILE NAME: binutils.c                                                   *
 *                                                                         *
 * PURPOSE: Helper functions for extracting and transforming bits          *
 *                                                                         *
 * DEVELOPMENT HISTORY:                                                    *
 *                                                                         *
 * Date          Author                Description of Changes              *
 * ----          ------                ----------------------              *
 * 20/05/2016    Tencho Tenev          Initial version                     *
 ***************************************************************************/

#include <stdint.h>
#include <assert.h>

#define BYTE 8
#define LSB 0xff
#define BYTE1 0xff00
#define BYTE2 0xff0000
#define MSB 0xff000000

/* Get N (count) bits to the right starting at position (pos)
 * in the value (val)
 *      @param val      Value to val used when extracting bits
 *      @param pos      Position to start extracting from [31;0]
 *      @param count    Numvalr of bits to take staring from pos
 *
 *      Constraints:
 *          pos - count >= 0
 */
uint32_t get_bits(uint32_t val, short pos, short count) {
    assert(pos + 1 - count >= 0);
    assert(pos >= 0 && pos < sizeof(uint32_t) * BYTE);

    /* Shift value so that rightmost wanted bit is in position 0
     * and then apply a mask of 1 on the region up to count */
    return ((val >> (pos + 1 - count)) & ~(~0 << count));
}

/*
 * Function : set_byte
 * Usage    : set_byte(&word, 24, 0xfa)
 * ------------------------------------
 * Sets the byte in a word starting at the given position starting
 */
void set_byte_at(uint32_t *src, short pos, uint8_t byte) {
    *src = (*src & ~(0xff << pos)) | (((uint32_t)byte) << pos);
}

/* Endianess conversion */
uint32_t toggle_endian(uint32_t val) {
    return ((val >> 3 * BYTE) & LSB)
           | ((val >> BYTE) & BYTE1)
           | ((val << BYTE) & BYTE2)
           | ((val << 3 * BYTE) & MSB);
}
