/* Module  : bsi_writer
 *
 * Usage   : Writes a Binary Source image
 *
 * Authors : Tencho Tenev
 */

/* dest is a pointer to an image location
 * src is a stream open for binary reading
 */

#ifndef BSI_WRITER_H_
#define BSI_WRITER_H_

#include <stdio.h>
#include "image.h"

// Pixels per image block
enum {BLOCK_SIZE = 10};

// Data blocks per line
enum {LINE_SIZE = 32};

int bsi_write(image_t **dest, FILE *src, long len);

#endif
