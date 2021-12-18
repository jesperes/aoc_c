
#include "compare.h"
#include <aoc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 100
#define HEIGHT 100
#define MAX_LOW_POINTS 500
#define INPUT_SIZE 10100

typedef struct {
    int x;
    int y;
} coord_t;

coord_t delta[] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

char ptr[INPUT_SIZE];

#define READ_POS(X, Y, ptr) (ptr[(Y) * (WIDTH + 1) + (X)])
#define CLEAR(X, Y, ptr) (ptr[(Y) * (WIDTH + 1) + (X)] = ' ')

int fill(coord_t coord) {
    int size = 0;

    switch (READ_POS(coord.x, coord.y, ptr)) {
    case '9':
    case ' ':
        return size;
    default:
        size += 1;
        CLEAR(coord.x, coord.y, ptr);
        for (int i = 0; i < 4; i++) {
            coord_t d = delta[i];
            coord_t adj = {coord.x + d.x, coord.y + d.y};
            if (adj.x >= 0 && adj.x < WIDTH && adj.y >= 0 && adj.y < WIDTH) {
                size += fill(adj);
            }
        }

        return size;
    }
}

aoc_result_t day9(char *orig_ptr, UNUSED int len) {
    aoc_result_t result = {0};
    coord_t low_points[MAX_LOW_POINTS];
    int curr_lp = 0;

    // Make a copy, since we are modifying it
    memcpy(ptr, orig_ptr, len);

    /*
     * Part 1
     */
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < HEIGHT; x++) {
            int n = READ_POS(x, y, ptr);
            bool is_low = true;
            for (int i = 0; i < 4; i++) {
                coord_t d = delta[i];
                coord_t adj = {x + d.x, y + d.y};
                if (adj.x >= 0 && adj.x < WIDTH && adj.y >= 0 &&
                    adj.y < WIDTH) {
                    int p = READ_POS(adj.x, adj.y, ptr);
                    if (p <= n) {
                        is_low = false;
                        break;
                    }
                }
            }

            if (is_low) {
                result.p1 += (n - '0') + 1;
                coord_t lp = {x, y};
                low_points[curr_lp++] = lp;
                assert(curr_lp < MAX_LOW_POINTS);
            }
        }
    }

    /*
     * Part 2
     */
    int num_lp = curr_lp;
    int sizes[MAX_LOW_POINTS] = {0};

    for (int i = 0; i < num_lp; i++) {
        sizes[i] = fill(low_points[i]);
    }

    qsort(sizes, num_lp, sizeof(*sizes), int_compare_desc);
    result.p2 = sizes[0] * sizes[1] * sizes[2];

    return result;
}
