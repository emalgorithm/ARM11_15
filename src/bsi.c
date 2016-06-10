#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "imager/image.h"
#include "imager/bsi_writer.h"

/*
 * Takes a path to a binary as first argument and a path to an output file
 * having one of the supported prefixes. The program then writes the binary
 * data to the new file.
 */
int main(int argc, char **argv) {
    char *src_path = argv[1];
    char *dest_path = argv[2];

    FILE *in = fopen(src_path, "rb");
    // Check

    image_t *out_img = malloc(sizeof(image_t));
    // Check

    // Get size of src file in bytes
    struct stat *st = malloc(sizeof(struct stat));
    stat(src_path, st);

    long size = st->st_size;

    free(st);

    // Must be a multiple of the word size
    assert(size % 4 == 0);

    if (bsi_write(&out_img, in, size / 4)) {
        perror("BSI writer error");
        exit(1);
    }

    if (image_write(dest_path, out_img, PPM_FORMAT)) {
        perror("Write image error");
        exit(1);
    }

    return EXIT_SUCCESS;
}
