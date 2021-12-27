#include "aoc.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 139
#define HEIGHT 137

#define POS(coord, input) input[(coord).y * (WIDTH + 1) + (coord).x]

typedef struct {
    uint8_t x;
    uint8_t y;
} coord_t;

typedef struct {
    coord_t *herd_elem; // pointer into the herd array
    coord_t to;
} move_t;

typedef struct {
    coord_t *array;
    int num_elems;
    char type;
} herd_t;

bool can_move(const char *input, herd_t *herd, coord_t *elem, move_t *move) {
    if (herd->type == '>') {
        move->to.x = (elem->x + 1) % WIDTH;
        move->to.y = elem->y;
    } else if (herd->type == 'v') {
        move->to.x = elem->x;
        move->to.y = (elem->y + 1) % HEIGHT;
    } else {
        assert(0);
    }

    if (POS(move->to, input) == '.') {
        move->herd_elem = elem;
        return true;
    }

    return false;
}

int move_herd(char *input, herd_t *herd) {
    move_t to_move[WIDTH * HEIGHT] = {0};
    int to_move_idx = 0;

    for (int i = 0; i < herd->num_elems; i++) {
        coord_t *elem = &herd->array[i];
        move_t *move = &to_move[to_move_idx];
        if (can_move(input, herd, elem, move)) {
            to_move_idx++;
        }
    }

    int num_to_move = to_move_idx;
    for (int i = 0; i < num_to_move; i++) {
        move_t *move = &to_move[i];
        POS(*move->herd_elem, input) = '.';
        POS(move->to, input) = herd->type;
        *move->herd_elem = move->to;
    }
    return num_to_move;
}

herd_t find_herd(const char *input, char c) {
    herd_t herd = {0};
    int len = WIDTH * HEIGHT;
    herd.array = calloc(len, sizeof(int));
    herd.type = c;
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            coord_t coord = {x, y};
            if (POS(coord, input) == c) {
                herd.array[herd.num_elems++] = coord;
            }
        }
    }
    return herd;
}

aoc_result_t day25(const char *input, int len) {
    aoc_result_t result = {0};
    char *buf = calloc(len, sizeof(char));
    memcpy(buf, input, len);

    herd_t eastwards = find_herd(input, '>');
    herd_t southwards = find_herd(input, 'v');

    int steps = 1;
    while (true) {
        int moves = move_herd(buf, &eastwards) + move_herd(buf, &southwards);
        if (moves == 0) {
            result.p1 = steps;
            break;
        } else {
            steps++;
        }
    }
    free(buf);
    free(eastwards.array);
    free(southwards.array);
    return result;
}