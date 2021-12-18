#include <aoc.h>
#include <string.h>

void assert_int64(const char *msg, int64_t actual, int64_t expected) {
    if (actual != expected) {
        printf("\033[31;1m*** ERROR %s: expected %ld, got %ld\033[0m\n", msg,
               expected, actual);
        assert(0);
    }
}

void assert_string(const char *msg, const char *actual, const char *expected) {
    if (strcmp(actual, expected) != 0) {
        printf("\033[31;1m*** ERROR %s: expected \"%s\", got \"%s\"\033[0m\n",
               msg, expected, actual);
        assert(0);
    }
}
