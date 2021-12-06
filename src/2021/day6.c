
#include <aoc.h>
#include <stdbool.h>
#include <stdlib.h>

// Buffer size to hold integers 0..8 (inclusive)
#define BUFSIZE 9

int64_t sum(int64_t array[]) {
    int64_t n = 0;
    for (int i = 0; i < BUFSIZE; i++) {
        n += array[i];
    }
    return n;
}

aoc_result_t day6(char *input, int len) {
    aoc_result_t result = {0};
    int64_t buf[BUFSIZE] = {0};

    for (int i = 0; i < len; i += 2) {
        buf[input[i] - '0']++;
    }

    for (int i = 0; i < 256; i++) {
        if (i == 80) {
            result.p1 = sum(buf);
        }

        int64_t buf0 = buf[0];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = buf0 + buf[7];
        buf[7] = buf[8];
        buf[8] = buf0;
    }

    result.p2 = sum(buf);
    return result;
}
