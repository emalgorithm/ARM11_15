#include <stdio.h>
#include "../../minunit.h"
#include "../../../../src/assembler/util/hashmap.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static char spec[] = "hashmap";
int tests_run = 0;

#define KEY_MAX_LENGTH (256)
#define KEY_PREFIX ("somekey")
#define KEY_COUNT (1024*1024)

typedef struct data_struct_s {
    char key_string[KEY_MAX_LENGTH];
    int number;
} data_struct_t;

void setup() { }

void tear_down() {}

static int test_hashmap() {

    map_t mymap;

    mymap = hashmap_new();

    char *str[50];
    int *value[50];

    for (int i = 'a'; i < 'z'; i++) {
        str[i - 'a'] = malloc(10);
        value[i - 'a'] = malloc(sizeof(int));
        *(value[i - 'a']) = i;
        strcpy(str[i - 'a'], "hello");
        str[i - 'a'][0] = i;

        hashmap_put(mymap, str[i - 'a'], (void *)(value[i - 'a']));

    }

    for (int i = 'a'; i < 'z'; i++) {
        char str[20];
        strcpy(str, "hello");
        str[0] = i;
        int *v;
        v = hashmap_get(mymap, str);
        mu_assert(*v == i);
    }

    /* Make sure that a value that wasn't in the map can't be found */
    char s[20];
    strcpy(s, "ello");
    int *v;
    v = hashmap_get(mymap, s);

    /* Make sure the value was not found */
    mu_assert(v == NULL);

    //Deallocate memory
    for (int i = 'a'; i < 'z'; i++) {
        free(str[i - 'a']);
        free(value[i - 'a']);
    }

    hashmap_free(mymap);

    return 0;
}

static int test_all() {
    printf("Running all tests for %s | ", spec);

    /* Test run in this order until one fails or all pass.
    * The position of the failed one is returned and printed.
    * If all pass 0 is returned.
    */


    mu_run_test(test_hashmap);

    return 0;
}

int main(int argc, char **argv) {

    int res = test_all();

    if (res != 0) {
        printf("TEST #%d FAILED.\n", res);
    } else {
        printf("%d passed\n", tests_run);
    }

    return res != 0;
}
