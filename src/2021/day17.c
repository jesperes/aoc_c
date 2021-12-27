#include "aoc.h"
#include <limits.h>
#include <stdbool.h>

// target area
int target_xmin = 29;
int target_xmax = 73;
int target_ymin = -248;
int target_ymax = -194;

// range of trajectories to try; found by trial and error
int range_xmin = 8;
int range_xmax = 73;
int range_ymin = -248;
int range_ymax = 248;

aoc_result_t day17(const char *input, int len) {
    aoc_result_t result = {0};
    int total_maxy = INT_MIN;
    int total_hits = 0;

    for (int tx = range_xmin; tx <= range_xmax; tx++) {
        for (int ty = range_ymin; ty <= range_ymax; ty++) {
            int x = 0;
            int y = 0;
            int dx = tx;
            int dy = ty;
            int maxy = INT_MIN;
            while (true) {
                if (x >= target_xmin && x <= target_xmax && y >= target_ymin &&
                    y <= target_ymax) {
                    total_hits++;
                    total_maxy = (maxy > total_maxy) ? maxy : total_maxy;
                    break;
                }
                if (y < target_ymin && dy < 0) {
                    break;
                }
                x += dx;
                y += dy;
                if (dx < 0) {
                    dx++;
                } else if (dx > 0) {
                    dx--;
                }
                dy--;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }

    result.p1 = total_maxy;
    result.p2 = total_hits;

    return result;
}
