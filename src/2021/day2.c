#include "aoc.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

enum Command { Forward, Up, Down };

aoc_result_t day2(char *input, UNUSED int len) {
    aoc_result_t result = {0};
    char *p = input;
    int depth1 = 0, depth2 = 0;
    int pos1 = 0, pos2 = 0;
    int aim = 0;
    while (1) {
        char *q = strchr(p, ' ');
        int l = q - p;
        enum Command cmd = -1;

        if (q == NULL)
            break;

        if (strncmp(p, "forward", l) == 0) {
            cmd = Forward;
        } else if (strncmp(p, "up", l) == 0) {
            cmd = Up;
        } else if (strncmp(p, "down", l) == 0) {
            cmd = Down;
        }

        q++; // Move q past the space, will not point to the integer
        int x = q[0] - '0'; // always a single digit

        switch (cmd) {
        case Forward:
            pos1 += x;
            pos2 += x;
            depth2 += aim * x;
            break;
        case Up:
            depth1 -= x;
            aim -= x;
            break;
        case Down:
            depth1 += x;
            aim += x;
            break;
        }

        // q points to the integer, so move p past the integer + the newline
        p = q + 2;
    }

    result.p1 = depth1 * pos1;
    result.p2 = depth2 * pos2;
    return result;
}
