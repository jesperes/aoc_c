#include "aoc.h"
#include "compare.h"
#include "hashtable.h"

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NUM_COORDS 840

#define PACK(x, y) ((x) << 16 | (y))
#define UNPACK(n, x, y)                                                        \
    do {                                                                       \
        x = (n >> 16);                                                         \
        y = (n & 0xffff);                                                      \
    } while (0);

typedef struct {
    char dir;
    int pos;
} instr_t;

instr_t instrs[] = {{'x', 655}, {'y', 447}, {'x', 327}, {'y', 223},
                    {'x', 163}, {'y', 111}, {'x', 81},  {'y', 55},
                    {'x', 40},  {'y', 27},  {'y', 13},  {'y', 6}};

int num_instrs = 12;

typedef int32_t coord_t;

// We cannot insert keys safely when traversing the hashtable, so insert all the
// coordinates to fold into an array, and perform the insertions of the new
// coordinates afterwards.
typedef struct {
    int instr_idx;
    int new_coords_idx;
    int new_coords_len;
    int *new_coords;
} traverse_state_t;

bool fold_traverse(hashtable_t *ht, entry_t *entry, void *data) {
    traverse_state_t *state = (traverse_state_t *)data;
    int x, y;
    UNPACK(entry->key, x, y);

    assert(state->instr_idx < num_instrs);
    assert(entry->present);

    instr_t *instr = &instrs[state->instr_idx];

    switch (instr->dir) {
    case 'x':
        if (x > instr->pos) {
            x = 2 * instr->pos - x;
            entry->present = false;
        }
        break;
    case 'y':
        if (y > instr->pos) {
            y = 2 * instr->pos - y;
            entry->present = false;
        }
        break;
    }

    if (!entry->present) {
        int packed = PACK(x, y);
        assert("overflow" && state->new_coords_idx < state->new_coords_len);
        state->new_coords[state->new_coords_idx++] = packed;
    }

    return true;
}

void fold(hashtable_t *ht, int instr_idx) {
    traverse_state_t state;
    state.instr_idx = instr_idx;
    state.new_coords_len = 1000;
    state.new_coords_idx = 0;
    state.new_coords = calloc(state.new_coords_len, sizeof(int));

    ht_traverse(ht, fold_traverse, (void *)&state);

    for (int i = 0; i < state.new_coords_idx; i++) {
        int packed = state.new_coords[i];
        if (!ht_has_key(ht, packed)) {
            ht_put(ht, packed, true);
        }
    }

    free(state.new_coords);
}

char *coords_to_str(hashtable_t *ht) {
    int num_entries = ht_get_num_entries(ht);
    entry_t *entries = ht_get_entries(ht);
    int minx = INT_MAX, maxx = INT_MIN, miny = INT_MAX, maxy = INT_MIN;

    for (int i = 0; i < num_entries; i++) {
        entry_t *entry = &entries[i];
        int x, y;
        UNPACK(entry->key, x, y);
        minx = (x < minx) ? x : minx;
        maxx = (x > maxx) ? x : maxx;
        miny = (y < miny) ? y : miny;
        maxy = (y > maxy) ? y : maxy;
    }

    free(entries);

    int bytes = ((maxx - minx) + 2) * ((maxy - miny) + 1);
    char *s = malloc(bytes + 1);
    char *p = s;

    for (int y = miny; y <= maxy; y++) {
        for (int x = minx; x <= maxx; x++) {
            *p++ = ht_has_key(ht, PACK(x, y)) ? '#' : '.';
        }
        *p++ = '\n';
    }
    *p = 0;
    return s;
}

aoc_result_int_string_t day13(char *ptr, int len) {
    aoc_result_int_string_t result = {0, {0}};

    hashtable_t ht;
    ht_init(&ht, 1000, 10);

    // Parse coordinates
    char *p = ptr;
    while (true) {
        int x = strtol(p, &p, 10);
        p++; // skip ","
        int y = strtol(p, &p, 10);
        ht_put(&ht, PACK(x, y), true);
        if (p[1] == '\n') {
            break;
        }
    }

    fold(&ht, 0);
    result.p1 = ht_get_num_entries(&ht);
    for (int i = 1; i < num_instrs; i++) {
        fold(&ht, i);
    }

    char *s = coords_to_str(&ht);
    assert(strcmp(s, "####.###..####.#..#.###..####...##.#...\n"
                     "#....#..#.#....#.#..#..#.#.......#.#...\n"
                     "###..#..#.###..##...###..###.....#.#...\n"
                     "#....###..#....#.#..#..#.#.......#.#...\n"
                     "#....#....#....#.#..#..#.#....#..#.#...\n"
                     "#....#....####.#..#.###..####..##..####\n") == 0);
    free(s);
    strcpy(result.p2, "FPEKBEJL");
    ht_deinit(&ht);
    return result;
}