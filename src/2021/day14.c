#include "aoc.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define NUM_RULES 100
// #define NUM_RULES 16
#define LETTERS 26
#define PAIR_COUNT ((LETTERS) * (LETTERS))
#define ORD(a) ((a) - 'A')
#define CHR(a) ((a) + 'A')
#define PACK(a, b) (ORD(a) * LETTERS + ORD(b))
#define UNPACK_A(n) CHR((n) / LETTERS)
#define UNPACK_B(n) CHR((n) % LETTERS)

void print_letter_freqs(int letter_counts[LETTERS]) {
    for (int i = 0; i < LETTERS; i++) {
        if (letter_counts[i] > 0) {
            printf("%c -> %d ", CHR(i), letter_counts[i]);
        }
    }
    printf("\n");
}

void print_pair_freqs(int pair_counts[PAIR_COUNT]) {
    for (int i = 0; i < PAIR_COUNT; i++) {
        int a = UNPACK_A(i);
        int b = UNPACK_B(i);
        int n = pair_counts[i];
        if (n >= 1) {
            printf("%c%c -> %d ", a, b, n);
        }
    }
    printf("\n");
}

int64_t get_answer(int64_t letter_counts[LETTERS]) {
    int64_t least_common = INT64_MAX;
    int64_t most_common = INT64_MIN;
    for (int i = 0; i < LETTERS; i++) {
        int64_t c = letter_counts[i];
        if (c == 0)
            continue;
        if (c > most_common)
            most_common = c;
        if (c < least_common)
            least_common = c;
    }
    return most_common - least_common;
}

aoc_result_t day14(const char *input, int len) {
    aoc_result_t result = {0};
    const char *p = input;
    char *q = strchr(p, '\n');
    char root[32] = {0};
    int rootlen = q - p;
    strncpy(root, p, rootlen);

    int64_t pair_counts[PAIR_COUNT] = {0};
    char rules[PAIR_COUNT] = {0};
    int64_t letter_counts[LETTERS] = {0};

    // Find letter frequencies
    for (int i = 0; i < rootlen; i++) {
        letter_counts[ORD(root[i])]++;
    }

    // Parse rules
    p = q + 2;
    for (int i = 0; i < NUM_RULES; i++) {
        q = strchr(p, '\n');
        assert(p[0] >= 'A' && p[0] <= 'Z');
        assert(p[1] >= 'A' && p[1] <= 'Z');
        rules[PACK(p[0], p[1])] = p[6];
        p = q + 1;
    }

    // Find pair counts
    for (int i = 0; i < rootlen - 1; i++) {
        pair_counts[PACK(root[i], root[i + 1])]++;
    }

    int generations = 40;

    for (int i = 0; i < generations; i++) {
        // Copy the pair_counts array; reads from it must not see the
        // modifications we are doing to it.
        int64_t pair_counts_orig[PAIR_COUNT];
        memcpy(pair_counts_orig, pair_counts, sizeof(int64_t) * PAIR_COUNT);

        for (int pair_idx = 0; pair_idx < PAIR_COUNT; pair_idx++) {
            if (rules[pair_idx] == 0 || pair_counts_orig[pair_idx] == 0) {
                continue;
            }

            // Consider rule AB -> C
            char a = UNPACK_A(pair_idx);
            char b = UNPACK_B(pair_idx);
            char c = rules[pair_idx];

            // How many AB pairs are there
            int64_t pair_count = pair_counts_orig[pair_idx];
            int ac = PACK(a, c);
            int cb = PACK(c, b);
            assert(ac >= 0 && ac < PAIR_COUNT);
            assert(cb >= 0 && cb < PAIR_COUNT);

            pair_counts[pair_idx] -= pair_count;
            pair_counts[ac] += pair_count;
            pair_counts[cb] += pair_count;
            letter_counts[ORD(c)] += pair_count;
        }

        if (i == 9) {
            result.p1 = get_answer(letter_counts);
        }
    }

    result.p2 = get_answer(letter_counts);
    return result;
}
