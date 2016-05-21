/* MinUnit testing framework for C
 * Reference : http://www.jera.com/techinfo/jtns/jtn002.html */

/* If passed is not 0 the number of this test will be returned
 * as an error code. */
#define mu_assert(passed) do { if (!(passed)) return tests_run; } while (0)

/* Takes a function and runs it incrementing the number of tests. */
#define mu_run_test(test) do { tests_run++; int status = test(); \
                                if (status) return status; } while (0)

extern int tests_run;
