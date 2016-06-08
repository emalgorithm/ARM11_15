#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>

typedef enum {
  PBM_FORMAT,
  PPM_FORMAT,
  PGM_FORMAT
} imageformat;

/* Constants */
enum {
  DEPTH = 255
};

enum {
  GRAY = 1,
  RGB = 3
};

enum {
  BUFFER_SIZE = 16
};

typedef struct {
  int width, height;
  int nChannels;
  int widthStep;
  int depth;
  uint8_t *pixelsData;
} image_t;

typedef enum{
  IMG_OK,
  IMG_OPEN_FAILURE,
  IMG_MISSING_FORMAT,
  IMG_INVALID_FORMAT,
  IMG_INSUFFICIENT_MEMORY,
  IMG_INVALID_SIZE,
  IMG_INVALID_DEPTH,
  IMG_READ_FAILURE,
  IMG_WRITE_FAILURE
} image_error_t;

void image_print_error(image_error_t error_code);
void image_free(image_t *image);
image_error_t image_read(const char *filename, image_t **image_ptr);
image_error_t image_write(const char *filenme, image_t *image,
                          imageformat format);
image_error_t init_image(image_t**, int, int, int, int);
void set_pixel(image_t *image, int x, int y, uint8_t colour);
uint8_t get_pixel(image_t *image, int x, int y);

#endif
