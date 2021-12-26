#include "aoc.h"
#include "btree.h"
#include "compare.h"
#include <limits.h>
#include <stdlib.h>

typedef union {
    uint64_t packed;
    struct {
        int16_t y;
        int16_t x;
        uint16_t dist;
        uint16_t pad;
    } fields;
} queue_entry_t;

queue_entry_t queue_entry(uint16_t dist, int16_t x, int16_t y) {
    queue_entry_t entry = {0};
    entry.fields.pad = 0;
    entry.fields.dist = dist;
    entry.fields.x = x;
    entry.fields.y = y;
    return entry;
}

#define WIDTH 100
#define HEIGHT 100
#define READ_POS(X, Y, input) (input[(Y) * (WIDTH + 1) + (X)] - '0')
#define IS_GOAL(X, Y, Tiles)                                                   \
    (((X) == WIDTH * Tiles - 1) && (((Y) == HEIGHT * Tiles - 1)))

#define GSCORE(x, y, tiles, gs) (gs[(y) * (WIDTH * tiles) + (x)])

struct {
    int dx, dy;
} day15_deltas[] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

int edge_weight(int16_t x, int16_t y, const char *input, int tiles) {
    int16_t x0 = x % WIDTH;
    int16_t y0 = y % HEIGHT;
    int tile_x = x / WIDTH;
    int tile_y = y / HEIGHT;
    int risk = READ_POS(x0, y0, input);
    return (risk + tile_x + tile_y - 1) % 9 + 1;
}

int64_t lower_bound_dist_to_goal(int16_t x, int16_t y, int tiles) {
    int16_t goal_x = WIDTH * tiles - 1;
    int16_t goal_y = HEIGHT * tiles - 1;
    return abs(goal_x - x) + abs(goal_y - y);
}

int queue_compare_fun(const void *a, const void *b, UNUSED void *data) {
    queue_entry_t *entry_a = (queue_entry_t *)a;
    queue_entry_t *entry_b = (queue_entry_t *)b;
    return (*int64_t_compare_asc)(&entry_a->packed, &entry_b->packed);
}

/*
 * A* search for the shortest path.
 */
int day15_find(const char *input, int tiles) {
    int width = WIDTH * tiles;
    int height = HEIGHT * tiles;

    uint16_t *gs = calloc(WIDTH * HEIGHT * tiles * tiles, sizeof(uint16_t));
    GSCORE(0, 0, tiles, gs) = 0;

    struct btree *queue =
        btree_new(sizeof(queue_entry_t), 0, queue_compare_fun, NULL);

    queue_entry_t start_elem =
        queue_entry(lower_bound_dist_to_goal(0, 0, tiles), 0, 0);
    btree_set(queue, &start_elem.packed);

    while (true) {
        queue_entry_t current = *(queue_entry_t *)btree_pop_min(queue);
        int16_t x = current.fields.x;
        int16_t y = current.fields.y;

        if (IS_GOAL(x, y, tiles)) {
            btree_free(queue);
            return current.fields.dist;
        }

        uint16_t current_gscore = GSCORE(x, y, tiles, gs);

        for (int i = 0; i < 4; i++) {
            int16_t xa = x + day15_deltas[i].dx;
            int16_t ya = y + day15_deltas[i].dy;

            if (!(xa >= 0 && xa < width && ya >= 0 && ya < height))
                continue;

            uint16_t nbr_old_gscore = GSCORE(xa, ya, tiles, gs);
            if (nbr_old_gscore == 0)
                nbr_old_gscore = UINT16_MAX;

            int ew = edge_weight(xa, ya, input, tiles);
            int maybe_new_gscore = current_gscore + ew;

            if (maybe_new_gscore < nbr_old_gscore) {
                GSCORE(xa, ya, tiles, gs) = maybe_new_gscore;
                int16_t new_dist =
                    maybe_new_gscore + lower_bound_dist_to_goal(xa, ya, tiles);
                queue_entry_t entry = queue_entry(new_dist, xa, ya);
                btree_set(queue, &entry.packed);
            }
        }
    }
}

aoc_result_t day15(const char *input, int len) {
    aoc_result_t result = {0};
    result.p1 = day15_find(input, 1);
    result.p2 = day15_find(input, 5);
    return result;
}
