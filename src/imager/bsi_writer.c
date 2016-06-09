/* Module  : bsi_writer
 *
 * Usage   : Writes a Binary Source image
 *
 * Authors : Tencho Tenev
 */

#include "bsi_writer.h"
#include <stdint.h>

struct sample {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

static const struct sample WHITE = {0xff, 0xff, 0xff};
static const struct sample BLACK = {0, 0, 0};
static const struct sample GREEN = {0, 0xff, 0};

static int write(image_t *img, FILE *src);
static int write_block(image_t *img, int x, int y, const struct sample *colour);


static uint32_t pix_width = 0;
static uint32_t pix_height = 0;

#define WIDTH 2 * LINE_SIZE + 1
#define HEIGHT(len) len * 2 + 2

#define PIX(blocks) blocks * BLOCK_SIZE

int bsi_write(image_t **dest, FILE *src, long len) {

    unsigned int width = WIDTH;
    unsigned int height = HEIGHT(len);

    pix_width = PIX(width);
    pix_height = PIX(height);

    // TODO: Error handling
    init_image(dest, pix_width, pix_height, RGB, DEPTH);

    write(*dest, src);

    fclose(src);

    return 0;
}

static int write(image_t *img, FILE *src) {
    static uint32_t mask = 1 << 31;

    for (uint32_t word = 0, line = 0; !feof(src); line += BLOCK_SIZE) {

        fread(&word, sizeof(uint32_t), 1, src);

        // Write block line
        for (int block_idx = 0, width = WIDTH - 1; block_idx < width; ++block_idx) {
            // Separator
            write_block(img, block_idx * BLOCK_SIZE * RGB, line, &GREEN);

            // Data
            const struct sample *data = ((word & mask) == 0) ? &WHITE : &BLACK;
            ++block_idx;
            write_block(img, block_idx * BLOCK_SIZE * RGB, line, data);

            // Take next data bit
            word <<= 1;
        }

        // Write separator line
        line += BLOCK_SIZE;
        for (int block_idx = 0, width = WIDTH - 1; block_idx < width; ++block_idx) {
            write_block(img, block_idx * BLOCK_SIZE * RGB, line, &GREEN);
        }
    }

    return 0;
}

static int write_block(image_t *img, int x, int y, const struct sample *colour) {
    int x_end = x + BLOCK_SIZE * RGB;
    int y_end = y + BLOCK_SIZE;

    while (x < x_end) {
        while (y < y_end) {
            set_pixel(img, x, y, colour->red);
            ++x;
            set_pixel(img, x, y, colour->green);
            ++x;
            set_pixel(img, x, y, colour->blue);
            ++y;
            x -= 2;
        }
        x += RGB;
        y -= BLOCK_SIZE;
    }

    return 0;
}
