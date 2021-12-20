#include "aoc.h"
#include "compare.h"
#include "hashtable.h"
#include "queue.h"
#include <limits.h>
#include <stdlib.h>

typedef union {
    uint64_t packed;
    struct {
        int16_t x;
        int16_t y;
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

typedef union {
    uint32_t packed;
    struct {
        int16_t x;
        int16_t y;
    } fields;
} gscore_entry_t;

gscore_entry_t gscore_entry(int x, int y) {
    gscore_entry_t entry;
    entry.fields.x = x;
    entry.fields.y = y;
    return entry;
}

#define WIDTH 100
#define QUEUE_SIZE WIDTH *WIDTH * 2
#define READ_POS(X, Y, input) (input[(Y) * (WIDTH + 1) + (X)] - '0')

int edge_weight(int16_t x, int16_t y, const char *input) {
    return READ_POS(x, y, input);
}

int64_t lower_bound_dist_to_goal(int16_t x, int16_t y) {
    int16_t goal_x = WIDTH - 1;
    int16_t goal_y = WIDTH - 1;
    return abs(goal_x - x) + abs(goal_y - y);
}

aoc_result_t day15(const char *input, int len) {
    aoc_result_t result = {0};

    assert(input[WIDTH] == '\n');
    assert(READ_POS(0, 0, input) == 2);
    assert(READ_POS(WIDTH - 1, 0, input) == 4);
    assert(READ_POS(0, WIDTH - 1, input) == 8);
    assert(READ_POS(WIDTH - 1, WIDTH - 1, input) == 4);

    hashtable_t gs;
    ht_init(&gs, 10000, 10);
    ht_put(&gs, gscore_entry(0, 0).packed, 0);

    queue_t queue;
    queue_init(&queue, QUEUE_SIZE);

    {
        // Queue self-test
        queue_entry_t entry_in = queue_entry(123, 456, 789);
        queue_entry_t entry_out = {0};
        queue_push(&queue, entry_in.packed);
        entry_out.packed = queue_pop(&queue);
        assert(entry_out.fields.dist == entry_in.fields.dist);
        assert(entry_out.fields.x == entry_in.fields.x);
        assert(entry_out.fields.y == entry_in.fields.y);
        assert(queue_is_empty(&queue));
    }

    queue_push(&queue,
               queue_entry(lower_bound_dist_to_goal(0, 0), 0, 0).packed);

    // int count = 20;

    while (!queue_is_empty(&queue)) {
        queue_entry_t current;
        current.packed = queue_pop(&queue);

        assert(current.fields.dist >= 0);
        assert(current.fields.x >= 0 && current.fields.y < WIDTH);
        assert(current.fields.y >= 0 && current.fields.y < WIDTH);

        unsigned int current_gscore = 0;
        assert(ht_get(&gs,
                      gscore_entry(current.fields.x, current.fields.y).packed,
                      &current_gscore));

        printf("current: dist=%u, x=%u, y=%u, packed=%016lx, gscore=%d\n",
               current.fields.dist, current.fields.x, current.fields.y,
               current.packed, current_gscore);

        if (current.fields.x == WIDTH - 1 && current.fields.y == WIDTH - 1) {
            printf("Found goal!\n");
            result.p1 = current.fields.dist;
            break;
        }

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0)) {
                    // skip diagonals
                    continue;
                }

                int16_t xa = current.fields.x + dx;
                int16_t ya = current.fields.y + dy;

                if (!(xa >= 0 && xa < WIDTH && ya >= 0 && ya < WIDTH))
                    continue;

                // Compute old gscore for neighbor, if any. Set to UINT_MAX if
                // not found.
                unsigned int nbr_old_gscore = 0;
                bool nbr_has_gscore =
                    ht_get(&gs, gscore_entry(xa, ya).packed, &nbr_old_gscore);
                if (!nbr_has_gscore)
                    nbr_old_gscore = UINT_MAX;

                // Compute possible new gscore from the current node's gscore
                // and the edge weight. If this is better than the previous
                // gscore, we have found a new better way to the current node.
                int ew = edge_weight(xa, ya, input);
                int maybe_new_gscore = current_gscore + ew;

                if (maybe_new_gscore < nbr_old_gscore) {
                    int better_score = maybe_new_gscore;
                    ht_put(&gs, gscore_entry(xa, ya).packed, better_score);
                    int16_t new_dist =
                        better_score + lower_bound_dist_to_goal(xa, ya);

                    queue_entry_t entry = queue_entry(new_dist, xa, ya);
                    printf("  push: dist=%u (%u+%ld) (ew=%d), x=%u, y=%u, "
                           "packed = "
                           "%016lx\n",
                           entry.fields.dist, better_score,
                           lower_bound_dist_to_goal(xa, ya), ew, entry.fields.x,
                           entry.fields.y, entry.packed);
                    queue_push(&queue, entry.packed);
                }
            }
        }

        printf("no more neighbors, sorting queue...\n");
        // queue_print(&queue);
        queue_sort(&queue);
        // queue_print(&queue);
    }

    return result;
}
