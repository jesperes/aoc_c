#include "aoc.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 139
#define HEIGHT 137

#define POS(x, y, input) input[(y) * (WIDTH + 1) + (x)]

typedef struct {
    int x;
    int y;
} coord_t;

typedef struct {
    coord_t from;
    coord_t to;
} move_t;

bool can_move(const char *input, int x, int y, char c, move_t *move) {
    int destx;
    int desty;
    if (c == '>') {
        destx = (x + 1) % WIDTH;
        desty = y;
    } else if (c == 'v') {
        destx = x;
        desty = (y + 1) % HEIGHT;
    } else {
        assert(0);
    }

    move->from.x = x;
    move->from.y = y;
    move->to.x = destx;
    move->to.y = desty;

    return POS(destx, desty, input) == '.';
}

int move_herd(char *input, char c) {
    move_t to_move[WIDTH * HEIGHT];
    int to_move_idx = 0;

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            char c0 = POS(x, y, input);
            move_t move;
            if (c0 == c && can_move(input, x, y, c, &move)) {
                to_move[to_move_idx++] = move;
            }
        }
    }

    int num_to_move = to_move_idx;
    for (int i = 0; i < num_to_move; i++) {
        move_t move = to_move[i];
        POS(move.from.x, move.from.y, input) = '.';
        POS(move.to.x, move.to.y, input) = c;
    }
    return num_to_move;
}

aoc_result_t day25(const char *input, int len) {
    aoc_result_t result = {0};
    char *buf = calloc(len, sizeof(char));
    memcpy(buf, input, len);
    int steps = 1;
    while (true) {
        int moves = move_herd(buf, '>') + move_herd(buf, 'v');
        if (moves == 0) {
            result.p1 = steps;
            break;
        } else {
            steps++;
        }
    }
    return result;
    free(buf);
}