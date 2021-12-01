#include "aoc2021.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

aoc_result_t day1() {
    aoc_result_t result = {0};
    FILE *f = fopen("inputs/2021/input01.txt", "rb");
    assert(f != NULL);
    char *linebuf = malloc(10);
    size_t n;
    ssize_t bytes;
    int a0 = INT_MAX, a1 = 0;
    int i = 0;
    int win[4];

    while ((bytes = getline(&linebuf, &n, f)) != -1) {

        linebuf[bytes - 1] = '\0'; // trim newline
        a1 = win[i % 4] = atoi(linebuf);
        if (a1 > a0) {
            result.p1++;
        }
        a0 = a1;

        if (i < 3) {
            i++;
            continue;
        }
        int w1 = win[(i + 1) % 4];
        int w2 = win[(i + 2) % 4];
        int w3 = win[(i + 3) % 4];
        int w4 = win[i % 4];

        if (w1 + w2 + w3 < w2 + w3 + w4) {
            result.p2++;
        }

        i++;
    }

    free(linebuf);
    return result;
}
