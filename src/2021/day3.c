#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_INTS 1000
#define WIDTH 12

#define READ_BIT(__input, __line, __bitpos)                                    \
    ((__input)[__line * (WIDTH + 1) + __bitpos] == '1')

int part1(char *input) {
    int zeros[WIDTH] = {0};
    int ones[WIDTH] = {0};

    for (int pos = 0; pos < WIDTH; pos++) {
        for (int i = 0; i < NUM_INTS; i++) {
            // Read the bits directly from the input array, no parsing
            // is necessary.
            if (READ_BIT(input, i, pos)) {
                ones[pos]++;
            } else {
                zeros[pos]++;
            }
        }
    }

    int gamma = 0, epsilon = 0;
    for (int pos = 0; pos < WIDTH; pos++) {
        int bitpos = WIDTH - pos - 1;
        if (ones[pos] > zeros[pos]) {
            gamma |= (1 << bitpos);
        } else if (ones[pos] < zeros[pos]) {
            epsilon |= (1 << bitpos);
        }
    }

    return gamma * epsilon;
}

// Count the number of ones and zeroes in the given position.
void make_freq(char *input, int pos, int *remaining, int num_remaining,
               int *ones, int *zeroes) {
    for (int i = 0; i < num_remaining; i++) {
        int row = remaining[i];
        if (READ_BIT(input, row, pos)) {
            (*ones)++;
        } else {
            (*zeroes)++;
        }
    }
}

int extract_num(char *input, int row) {
    return strtol(input + row * (WIDTH + 1), NULL, 2);
}

enum Criteria { Oxygen, CO2 };

int life_support_rating(char *input, UNUSED enum Criteria criteria) {
    // This is an array containing the indexes of the numbers still remaining
    int remaining[NUM_INTS] = {0};
    int num_remaining = NUM_INTS;

    // Initialize the "remaining" array with all numbers
    for (int i = 0; i < num_remaining; i++) {
        remaining[i] = i;
    }

    for (int pos = 0; pos < WIDTH; pos++) {
        int rem_pos = 0;
        int ones = 0;
        int zeroes = 0;

        // Compute the number of ones and zeroes for this position
        make_freq(input, pos, remaining, num_remaining, &ones, &zeroes);

        for (int i = 0; i < num_remaining; i++) {
            int row = remaining[i];
            if ((criteria == Oxygen) ? (ones >= zeroes) : (ones < zeroes)) {
                if (READ_BIT(input, row, pos)) {
                    remaining[rem_pos++] = row;
                }
            } else {
                if (!READ_BIT(input, row, pos)) {
                    remaining[rem_pos++] = row;
                }
            }
        }

        num_remaining = rem_pos;
        if (rem_pos == 1) {
            break;
        }
    }

    return extract_num(input, remaining[0]);
}

int part2(char *input) {
    int oxygen = life_support_rating(input, Oxygen);
    int co2 = life_support_rating(input, CO2);
    return oxygen * co2;
}

aoc_result_t day3(UNUSED char *input, UNUSED int len) {
    aoc_result_t result = {0};
    result.p1 = part1(input);
    result.p2 = part2(input);
    return result;
}