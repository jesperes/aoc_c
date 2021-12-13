#include <aoc.h>
#include <stdbool.h>
#include <stdlib.h>

#define WIDTH 10

typedef struct {
    int dx;
    int dy;
} delta_t;

#define SET_FLASHED(x) ((x) = -1)
#define IS_FLASHED(x) ((x) == -1)
#define RESET_FLASHED(x) ((x) = 0)

delta_t deltas[] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0},
                    {1, 0},   {-1, 1}, {0, 1},  {1, 1}};

typedef int8_t grid_type;
typedef grid_type grid_t[WIDTH][WIDTH];

void maybe_flash(int x, int y, grid_t grid) {
    int val = grid[x][y];

    if (val >= 10) {
        SET_FLASHED(grid[x][y]);

        for (int i = 0; i < 8; i++) {
            delta_t d = deltas[i];
            int xadj = x + d.dx;
            int yadj = y + d.dy;

            if (xadj >= 0 && xadj < WIDTH && yadj >= 0 && yadj < WIDTH &&
                !IS_FLASHED(grid[xadj][yadj])) {
                grid[xadj][yadj]++;
                maybe_flash(xadj, yadj, grid);
            }
        }
    }
}

int do_one_iter(grid_t grid) {
    int num_flashes = 0;

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[x][y]++;
        }
    }

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < WIDTH; x++) {
            maybe_flash(x, y, grid);
        }
    }

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (IS_FLASHED(grid[x][y])) {
                num_flashes++;
                RESET_FLASHED(grid[x][y]);
            }
        }
    }

    return num_flashes;
}

aoc_result_t day11(char *ptr, int len) {
    aoc_result_t result = {0};
    grid_t grid;

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[x][y] = ptr[y * (WIDTH + 1) + x] - '0';
        }
    }

    int i = 0;

    while (true) {
        i++;
        int flashes = do_one_iter(grid);
        if (i <= 100) {
            result.p1 += flashes;
        }
        if (flashes == 100) {
            result.p2 = i;
            break;
        }
    }

    return result;
}
