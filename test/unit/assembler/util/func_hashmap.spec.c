#include <stdio.h>
#include "../../minunit.h"
#include "../../../../src/assembler/util/func_hashmap.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static char spec[] = "func_hashmap";
int tests_run = 0;

#define KEY_MAX_LENGTH (256)
#define KEY_PREFIX ("somekey")
#define KEY_COUNT (1024*1024)

void setup() { }

void tear_down() {}

int f1(void) {
    return 0;
}

int f2(void) {
    return 1;
}

static int test_hashmap() {

    func_map_t mymap;

    mymap = func_hashmap_new();

    char str1[10] = "hello";
    func_hashmap_put(mymap, str1, (func_t)f1);

    char str2[10] = "ciao";
    func_hashmap_put(mymap, str2, (func_t)f2);

    int (*func1)(void) = (int (*)(void)) func_hashmap_get(mymap, "hello");

    int (*func2)(void) = (int (*)(void)) func_hashmap_get(mymap, "ciao");

    mu_assert(func1() == 0);
    mu_assert(func2() == 1);


    func_hashmap_free(mymap);

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
