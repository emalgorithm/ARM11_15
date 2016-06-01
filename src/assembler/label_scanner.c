#include "label_scanner.h"
#include "util/hashmap.h"
#include "util/tokeniser.h"
#include <stdlib.h>

#define MAX_LABEL_LENGTH 50
#define MAX_NUM_OF_LABELS 1000

static map_t label_map;
static char *labels[MAX_NUM_OF_LABELS];
static uint32_t *label_addresses[MAX_NUM_OF_LABELS];

static uint32_t num_of_labels = 0;

void scan_init(const char *path) {
    label_map = hashmap_new();
    tokinit(path);

    for(;;) {
        labels[num_of_labels] = malloc(MAX_LABEL_LENGTH);
        label_addresses[num_of_labels] = malloc(sizeof(uint32_t));

        *(label_addresses[num_of_labels]) = toklabel(labels[num_of_labels]);

        //No more labels present
        if (*(label_addresses[num_of_labels]) == -1) {
            break;
        }

        hashmap_put(label_map, labels[num_of_labels], (any_t)label_addresses[num_of_labels]);

        num_of_labels++;
    }
}


void scan_terminate(const char *path) {

    for (int i = 0; i < num_of_labels; i++) {
        free(labels[i]);
        free(label_addresses[i]);
    }

}


uint32_t get_label_address(const char *label) {
    return *((uint32_t *)hashmap_get(label_map, label));
}
