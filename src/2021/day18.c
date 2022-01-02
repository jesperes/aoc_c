#include "aoc.h"
#include "utils.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 100

typedef struct node {
    bool is_literal;
    int value;
    int depth;
    struct node *left;
    struct node *right;
} node_t;

typedef struct {
    int value;
    int depth;
} digit_t;

typedef struct {
    digit_t *digits;
    digit_t *copy_buf;
    int len;
} number_t;

number_t new_number() {
    number_t number = {0};
    number.digits = calloc(100, sizeof(digit_t));
    number.copy_buf = calloc(100, sizeof(digit_t));
    number.len = 0;
    return number;
}

void free_number(number_t *number) {
    free(number->digits);
    free(number->copy_buf);
}

number_t parse_number(char *str) {
    int depth = 0;
    number_t number = new_number();
    int j = 0;

    for (int i = 0; str[i] != 0; i++) {
        char c = str[i];
        switch (c) {
        case '[':
            depth++;
            break;
        case ']':
            depth--;
            break;
        case ',':
            break;
        default: {
            assert(isdigit(c));
            char *p = &str[i];
            number.digits[j].depth = depth;
            number.digits[j].value = strtol(p, &p, 10);
            i = (p - str) - 1;
            j++;
            break;
        }
        }
    }

    number.len = j;
    return number;
}

// Inspired by https://github.com/aldanor/aoc-2021/blob/master/src/day18/mod.rs,
// but translated to C.
void fast_reduce(number_t *number, bool explode_only) {
    int i = 0; // index into the source buffer
    int j = 0; // index into the target buffer

    if (explode_only && number->digits[0].depth == 5) {
        if (number->len != 2) {
            number->digits[2].value += number->digits[1].value;
        }
        digit_t zero = {0, 4};
        number->copy_buf[j++] = zero;
        i = 2;
    }

    while (i < number->len) {
        digit_t *digit = &number->digits[i];
        // printf("i = %d, digit = %d at depth %d\n", i, digit->value,
        //        digit->depth);
        if (digit->depth == 5) {
            // explode
            assert(number->digits[i + 1].depth == 5);
            int left_value = number->digits[i].value;
            int right_value = number->digits[i + 1].value;
            printf("exploding (%d,%d)\n", left_value, right_value);
            digit_t zero = {0, 4};
            number->digits[i + 1] = zero;
            if (i + 2 < number->len) {
                number->digits[i + 2].value += right_value;
            }

            if (explode_only) {
                number->copy_buf[j - 1].value += left_value;
                i++;
            } else if (j > 0) {
                number->digits[i] = number->copy_buf[--j];
                number->digits[i].value += left_value;
            } else {
                i++;
            }
        } else if (!explode_only && digit->value >= 10) {
            // split
            digit_t left = {digit->value >> 1, digit->depth + 1};
            digit_t right = {digit->value - left.value, left.depth};
            printf("splitting %d -> %d,%d\n", digit->value, left.value,
                   right.value);
            if (i != 0) {
                number->digits[i - 1] = left;
                number->digits[i] = right;
                i--;
            } else {
                number->digits[0] = left;
                number->digits[1] = right;
            }
        } else {
            number->copy_buf[j++] = number->digits[i++];
        }
    }

    digit_t *tmp = number->digits;
    number->digits = number->copy_buf;
    number->copy_buf = tmp;
    number->len = j;
}

void reduce(number_t *number) {
    fast_reduce(number, true);
    fast_reduce(number, false);
}

void add_and_reduce(number_t *number, number_t *other) {
    for (int i = 0; i < other->len; i++) {
        number->digits[number->len + i] = other->digits[i];
    }
    number->len += other->len;
    for (int i = 0; i < number->len; i++) {
        number->digits[i].depth++;
    }
    reduce(number);
}

int magnitude(number_t *number) {
    digit_t stack[MAX_DIGITS];
    int p = 0;

    for (int i = 0; i < number->len; i++) {
        stack[p++] = number->digits[i];
        while (p > 1) {
            digit_t left = stack[p - 2];
            digit_t right = stack[p - 1];
            if (left.depth == right.depth) {
                stack[p - 2].value = left.value * 3 + right.value * 2;
                stack[p - 2].depth = left.depth - 1;
                p--;
            } else {
                break;
            }
        }
    }

    assert(p == 1);
    return stack[0].value;
}

node_t to_nodes(number_t *number) {
    node_t *nodes = calloc(number->len, sizeof(node_t));
    int numnodes = number->len;
    for (int i = 0; i < number->len; i++) {
        nodes[i].is_literal = true;
        nodes[i].depth = number->digits[i].depth;
        nodes[i].value = number->digits[i].value;
        nodes[i].left = NULL;
        nodes[i].right = NULL;
    }

outer:
    while (numnodes > 1) {
        // printf("numnodes = %d\n", numnodes);
        // for (int i = 0; i < numnodes; i++) {
        //     printf("%d (depth %d)\n", nodes[i].value, nodes[i].depth);
        // }
        for (int i = 0; i < numnodes - 1; i++) {
            if (nodes[i].depth == nodes[i + 1].depth) {
                // printf("Collapsed pair %d,%d at depth %d\n", nodes[i].value,
                //        nodes[i + 1].value, nodes[i].depth);
                node_t *left = malloc(sizeof(node_t));
                *left = nodes[i];
                node_t *right = malloc(sizeof(node_t));
                *right = nodes[i + 1];
                nodes[i].depth--;
                nodes[i].is_literal = false;
                nodes[i].value = -1;
                nodes[i].left = left;
                nodes[i].right = right;
                int remaining_nodes = numnodes - (i + 2);
                memmove(&nodes[i + 1], &nodes[i + 2],
                        remaining_nodes * sizeof(node_t));
                numnodes--;
                goto outer;
            }
        }
    }

    node_t root = nodes[0];
    free(nodes);
    return root;
}

void print_nodes(node_t *node) {
    if (node->is_literal) {
        printf("%d", node->value);
    } else {
        printf("[");
        print_nodes(node->left);
        printf(",");
        print_nodes(node->right);
        printf("]");
    }
}

void print_number(number_t *number) {
    node_t nodes = to_nodes(number);
    print_nodes(&nodes);
    printf("\n");
}

int sum_numbers(char *input, int len) {
    if (len == -1)
        len = strlen(input);
    int numlines = 0;
    char **lines = split_input_to_lines(input, len, &numlines);

    number_t a = parse_number(lines[0]);

    for (int i = 1; i < numlines; i++) {
        number_t b = parse_number(lines[i]);
        add_and_reduce(&a, &b);
    }

    print_number(&a);

    return magnitude(&a);
}

aoc_result_t day18(char *input, int len) {
    aoc_result_t result = {0};

    // number_t number = parse_number("[1,[2,[3,[4,[5,6]]]]]");
    // print_number(&number);
    // reduce(&number);
    // print_number(&number);
    assert(0 == sum_numbers("[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]\n"
                            "[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]\n"
                            "[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]\n",
                            -1));

    result.p1 = sum_numbers(input, len);
    return result;
}
