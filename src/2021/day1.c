#include "aoc.h"
#include <limits.h>
#include <stdlib.h>

aoc_result_t day1(char *input, UNUSED int len) {
    aoc_result_t result = {0};
    char *p = input;
    int a0 = INT_MAX, a1 = 0;
    int i = 0;
    int win[4];

    while (1) {
        a1 = win[i % 4] = strtol(p, &p, 10);
        if (a1 == 0)
            break;
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
    return result;
}
