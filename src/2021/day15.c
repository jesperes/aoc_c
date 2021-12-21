#include "aoc.h"
#include "compare.h"
#include "hashtable.h"
#include "queue.h"
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
#define HEIGHT 100
#define QUEUE_SIZE (WIDTH) * (WIDTH) * (100)
#define READ_POS(X, Y, input) (input[(Y) * (WIDTH + 1) + (X)] - '0')
#define IS_GOAL(X, Y, Tiles)                                                   \
    (((X) == WIDTH * Tiles - 1) && (((Y) == HEIGHT * Tiles - 1)))

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

int find(const char *input, int tiles) {

    hashtable_t gs;
    ht_init(&gs, 10000, 100);
    ht_put(&gs, gscore_entry(0, 0).packed, 0);

    queue_t queue;
    queue_init(&queue, QUEUE_SIZE);

    int width = WIDTH * tiles;
    int height = HEIGHT * tiles;

#if 0
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
#endif

    queue_push(&queue,
               queue_entry(lower_bound_dist_to_goal(0, 0, tiles), 0, 0).packed);

    while (!queue_is_empty(&queue)) {
        queue_entry_t current;
        current.packed = queue_pop(&queue);

        assert(current.fields.dist >= 0);
        assert(current.fields.x >= 0 && current.fields.y < width);
        assert(current.fields.y >= 0 && current.fields.y < height);

        // printf("queue size: %d\n", QUEUE_NUM_ELEMS(&queue));

        // printf("curr={%d,%d} dist=%u packed=%lx\n", current.fields.x,
        //        current.fields.y, current.fields.dist, current.packed);

        if (IS_GOAL(current.fields.x, current.fields.y, tiles)) {
            return current.fields.dist;
        }

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {

                if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0)) {
                    // skip diagonals
                    continue;
                }

                int16_t xa = current.fields.x + dx;
                int16_t ya = current.fields.y + dy;

                if (!(xa >= 0 && xa < width && ya >= 0 && ya < height)) {
                    continue;
                }

                unsigned int current_gscore = 0;
                assert(ht_get(
                    &gs,
                    gscore_entry(current.fields.x, current.fields.y).packed,
                    &current_gscore));

                //                printf("%d,%d\n", dx, dy);

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
                int ew = edge_weight(xa, ya, input, tiles);
                int maybe_new_gscore = current_gscore + ew;

                if (maybe_new_gscore < nbr_old_gscore) {
                    int better_score = maybe_new_gscore;
                    // printf("current_gscore {%d,%d} = %d, edge_weight {%d,%d}
                    // = "
                    //        "%d\n",
                    //        current.fields.x, current.fields.y,
                    //        current_gscore, xa, ya, ew);

                    // printf("gscore {%d,%d} = %d\n", xa, ya, better_score);
                    ht_put(&gs, gscore_entry(xa, ya).packed, better_score);

                    int16_t new_dist =
                        better_score + lower_bound_dist_to_goal(xa, ya, tiles);

                    queue_entry_t entry = queue_entry(new_dist, xa, ya);

                    // printf("push={%d,%d} dist=%u (better=%u) packed=%lx\n",
                    //        entry.fields.x, entry.fields.y, entry.fields.dist,
                    //        better_score, entry.packed);

                    queue_push(&queue, entry.packed);
                }
            }
        }

        queue_sort(&queue);
    }
    assert("queue is empty without finding a result" && 0);
}

aoc_result_t day15(const char *input, int len) {
    aoc_result_t result = {0};

    assert(input[WIDTH] == '\n');
    assert(READ_POS(0, 0, input) == 2);
    assert(READ_POS(WIDTH - 1, 0, input) == 4);
    assert(READ_POS(0, WIDTH - 1, input) == 8);
    assert(READ_POS(WIDTH - 1, WIDTH - 1, input) == 4);

    result.p1 = find(input, 1);
    result.p2 = find(input, 5);
    return result;
}
