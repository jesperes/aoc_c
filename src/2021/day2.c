#include "aoc.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

aoc_result_t day2(char *input, UNUSED int len) {
    aoc_result_t result = {0};
    char *p = input;
    int depth1 = 0, depth2 = 0;
    int pos1 = 0, pos2 = 0;
    int aim = 0;

    while (true) {
        char *q = strchr(p, ' ');
        if (q == NULL)
            break;
        q++; // Move q past the space, will not point to the integer
        int x = q[0] - '0'; // always a single digit

        if (p[0] == 'f') {
            pos1 += x;
            pos2 += x;
            depth2 += aim * x;
        } else if (p[0] == 'u') {
            depth1 -= x;
            aim -= x;
        } else if (p[0] == 'd') {
            depth1 += x;
            aim += x;
        }

        // q points to the integer, so move p past the integer + the newline
        p = q + 2;
    }

    result.p1 = depth1 * pos1;
    result.p2 = depth2 * pos2;
    return result;
}
