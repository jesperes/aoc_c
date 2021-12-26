#include "compare.h"
#include <aoc.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LHS_DIGITS 20
#define RHS_DIGITS 5
#define NUM_DISPLAYS 200
#define IS_SEGMENT(c) ((c) >= 'a' && (c) <= 'g')

#define BITMASK7 0x7f
#define IS_UNIQUE(mask)                                                        \
    ((mask) == 1 || (mask) == (1 << 1) || (mask) == (1 << 2) ||                \
     (mask) == (1 << 3) || (mask) == (1 << 4) || (mask) == (1 << 5) ||         \
     (mask) == (1 << 6))

typedef struct {
    char lhs[MAX_LHS_DIGITS][8];
    int num_lhs;
    char rhs[RHS_DIGITS][8];
    int num_rhs;
} display_t;

char mask_to_char(char mask) {
    mask = mask & 0x7f;
    if (mask == 1)
        return 'a';
    else if (mask == 1 << 1)
        return 'b';
    else if (mask == 1 << 2)
        return 'c';
    else if (mask == 1 << 3)
        return 'd';
    else if (mask == 1 << 4)
        return 'e';
    else if (mask == 1 << 5)
        return 'f';
    else if (mask == 1 << 6)
        return 'g';
    else
        return '?';
}

char *next_digit(char *p, char *digit) {
    int i = 0;
    char *q = p;
    for (; IS_SEGMENT(*q); q++) {
        digit[i++] = *q;
    }
    digit[i] = 0;
    return q;
}

void print_display(display_t *display) {
    for (int i = 0; i < display->num_lhs; i++) {
        printf("%s ", display->lhs[i]);
    }
    printf("| ");
    for (int i = 0; i < display->num_rhs; i++) {
        printf("%s ", display->rhs[i]);
    }
    printf("\n");
}

char *process_line(char *ptr, display_t *display) {
    char *p = ptr;
    bool is_lhs = true;

    while (true) {
        char *digit = (is_lhs) ? display->lhs[display->num_lhs++]
                               : display->rhs[display->num_rhs++];
        p = next_digit(p, digit);
        if (isspace(*p) && *p != '\n') {
            p++;
        }

        if (*p == '|') {
            is_lhs = false;
            p += 2;
        } else if (*p == '\n') {
            return p + 1;
        }
    }
}

void print_mapping(char mapping[7]) {
    printf("Current mapping: ");
    for (int i = 0; i < 7; i++) {
        printf("%c => ", 'a' + i);
        for (int j = 0; j < 7; j++) {
            if (mapping[i] & (1 << j)) {
                printf("%c", 'a' + j);
            }
        }
        printf(", ");
    }
    printf("\n");
}

// For all unique mappings, i.e. a => b, remove "b" from all other mappings.
void prune_fixed(char mapping[7]) {
    for (int i = 0; i < 7; i++) {
        char m = mapping[i];
        if (IS_UNIQUE(m & 0x7f)) {
            for (int j = 0; j < 7; j++) {
                if (j == i)
                    continue;
                mapping[j] &= ~m;
            }
        }
    }
}

void deduce(display_t *display, char mapping[7]) {
    for (int i = 0; i < 7; i++) {
        mapping[i] = 0xff;
    }

    for (int i = 0; i < display->num_lhs; i++) {
        char *p = display->lhs[i];

        switch (strlen(p)) {
        case 2: {
            // Remove all but 'cf'
            char bits = (1 << ('c' - 'a')) | (1 << ('f' - 'a'));
            mapping[p[0] - 'a'] &= bits;
            mapping[p[1] - 'a'] &= bits;
            // Remove cf for all chars not in pattern
            for (int i = 0; i < 7; i++) {
                if (p[0] - 'a' != i && p[1] - 'a' != i) {
                    mapping[i] &= ~bits;
                }
            }
            break;
        }
        case 3: {
            // Remove all but 'acf'
            char bits = 1 << ('c' - 'a') | 1 << ('f' - 'a') | 1;
            mapping[p[0] - 'a'] &= bits;
            mapping[p[1] - 'a'] &= bits;
            mapping[p[2] - 'a'] &= bits;

            // Remove acf for all chars not in pattern
            char bits2 = ~((1 << ('c' - 'a')) | (1 << ('f' - 'a')) | 1);

            for (int i = 0; i < 7; i++) {
                if (p[0] - 'a' != i && p[1] - 'a' != i && p[2] - 'a' != i) {
                    mapping[i] &= bits2;
                }
            }

            break;
        }
        case 4: {
            // Remove all but 'bcdf'
            char bits = 1 << ('b' - 'a') | 1 << ('c' - 'a') | 1 << ('d' - 'a') |
                        1 << ('f' - 'a');
            mapping[p[0] - 'a'] &= bits;
            mapping[p[1] - 'a'] &= bits;
            mapping[p[2] - 'a'] &= bits;
            mapping[p[3] - 'a'] &= bits;

            // Remove bcdf for all chars not in pattern
            char bits2 = ~(1 << ('b' - 'a') | 1 << ('c' - 'a') |
                           1 << ('d' - 'a') | 1 << ('f' - 'a'));
            for (int i = 0; i < 7; i++) {
                if (p[0] - 'a' != i && p[1] - 'a' != i && p[2] - 'a' != i &&
                    p[3] - 'a' != i) {
                    mapping[i] &= bits2;
                }
            }

            break;
        }
        case 6: {
            // for 6-segment digits, only c, d, or e can be off, so
            // all others (a, b, f, g) are always on. I.e., if a segment
            // is *missing* in the pattern, we know that it is one of c, d, or
            // e.
            bool present[7] = {0};
            for (int i = 0; i < 7; i++) {
                present[p[i] - 'a'] = true;
            }
            for (int i = 0; i < 7; i++) {
                if (!present[i]) {
                    // Element is not present in pattern, so remove all but c,
                    // d, e
                    mapping[i] &= (1 << ('c' - 'a') | (1 << ('d' - 'a')) |
                                   1 << ('e' - 'a'));
                }
            }

            break;
        }
        }
        prune_fixed(mapping);
    }
}

char mapchar(char mapping[8], char c) {
    int mask = mapping[c - 'a'] & 0x7f;
    return mask_to_char(mask);
}

int segments_to_num(char mapping[7], char *digit) {
    int len = strlen(digit);
    char remapped[8] = {0};
    int k = 0;

    for (int i = 0; i < len; i++) {
        remapped[k++] = mapchar(mapping, digit[i]);
    }

    qsort(remapped, len, 1, char_compare_asc);

    if (strcmp(remapped, "abcefg") == 0) {
        return 0;
    } else if (strcmp(remapped, "cf") == 0) {
        return 1;
    } else if (strcmp(remapped, "acdeg") == 0) {
        return 2;
    } else if (strcmp(remapped, "acdfg") == 0) {
        return 3;
    } else if (strcmp(remapped, "bcdf") == 0) {
        return 4;
    } else if (strcmp(remapped, "abdfg") == 0) {
        return 5;
    } else if (strcmp(remapped, "abdefg") == 0) {
        return 6;
    } else if (strcmp(remapped, "acf") == 0) {
        return 7;
    } else if (strcmp(remapped, "abcdefg") == 0) {
        return 8;
    } else if (strcmp(remapped, "abcdfg") == 0) {
        return 9;
    } else {
        assert(0);
        return -1;
    }
}

aoc_result_t day8(char *ptr, UNUSED int len) {
    aoc_result_t result = {0};
    display_t displays[NUM_DISPLAYS] = {0};
    char *p = ptr;

    for (int i = 0; i < NUM_DISPLAYS; i++) {
        p = process_line(p, &displays[i]);

        /*
         * Part 1
         */
        for (int j = 0; j < 4; j++) {
            int l = strlen(displays[i].rhs[j]);
            if (l == 2 || l == 3 || l == 4 || l == 7) {
                result.p1++;
            }
        }

        /*
         * Part 2
         */
        char mapping[7];
        deduce(&displays[i], mapping);

        int factor = 1000;
        int display_value = 0;
        for (int j = 0; j < 4; j++) {
            display_value +=
                factor * segments_to_num(mapping, displays[i].rhs[j]);
            factor /= 10;
        }

        result.p2 += display_value;
    }

    return result;
}
