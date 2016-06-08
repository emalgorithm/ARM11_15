#include <stdlib.h>
#include <stdio.h>
#include "imager/image.h"

/*
 * Takes a path to a binary as first argument and a path to an output file
 * having one of the supported prefixes. The program then writes the binary
 * data to the new file.
 */
int main(int argc, char **argv) {
    char *src_path = argv[1];
    char *dest_path = argv[2];

    FILE *in = fopen(src_path, "rb");

    fseek(in, 0L, SEEK_END);
    int size = ftell(in);
    rewind(in);

    int width = size / 4;
    int height = 5;
    uint8_t buffer[4];

    image_t *out_img = malloc(sizeof(image_t));

    if(init_image(&out_img, width, height, GRAY, DEPTH)) {
      perror("Init image error");
      exit(1);
    }

    for (size_t x = 0; x < width - 4;) {
      for (size_t y = 0; y < height - 4;) {
        fread(buffer, sizeof(buffer), 1, in);

        if(feof(in)) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            set_pixel(out_img, x++, y++, buffer[i]);
        }
      }
    }

    if(image_write(dest_path, out_img, PGM_FORMAT)) {
      perror("Write image error");
      exit(1);
    }
}
