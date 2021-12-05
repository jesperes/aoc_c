#include "aoc.h"
#include <stdlib.h>

#define NUM_LINES 500

typedef struct {
    int x0;
    int y0;
    int x1;
    int y1;
} line_t;

void ascending(int a, int b, int *start, int *end) {
    if (a < b) {
        *start = a;
        *end = b;
    } else {
        *start = b;
        *end = a;
    }
}

int sign(int x) { return (x < 0) ? -1 : 1; }

aoc_result_t day5(UNUSED char *input, UNUSED int len) {
    aoc_result_t result = {0};
    line_t lines[NUM_LINES];
    char p1[1000][1000] = {0};
    char p2[1000][1000] = {0};
    char *p = input;

    for (int i = 0; i < NUM_LINES; i++) {
        lines[i].x0 = strtol(p, &p, 10);
        p++; // skip ","
        lines[i].y0 = strtol(p, &p, 10);
        p += 3; // skip "-> "
        lines[i].x1 = strtol(p, &p, 10);
        p++; // skip ",""
        lines[i].y1 = strtol(p, &p, 10);
    }

    for (int i = 0; i < NUM_LINES; i++) {
        line_t line = lines[i];
        if (line.x0 == line.x1) {
            int y0, y1;
            int x = line.x0;
            ascending(line.y0, line.y1, &y0, &y1);
            for (int y = y0; y <= y1; y++) {
                p1[x][y]++;
                p2[x][y]++;
            }
        } else if (line.y0 == line.y1) {
            int x0, x1;
            int y = line.y0;
            ascending(line.x0, line.x1, &x0, &x1);
            for (int x = x0; x <= x1; x++) {
                p1[x][y]++;
                p2[x][y]++;
            }
        } else {
            int dx = sign(line.x1 - line.x0);
            int dy = sign(line.y1 - line.y0);
            int len = abs(line.x1 - line.x0);
            for (int l = 0; l <= len; l++) {
                int x = line.x0 + l * dx;
                int y = line.y0 + l * dy;
                p2[x][y]++;
            }
        }
    }

    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 1000; j++) {
            if (p1[i][j] >= 2) {
                result.p1++;
            }
            if (p2[i][j] >= 2) {
                result.p2++;
            }
        }
    }

    return result;
}
