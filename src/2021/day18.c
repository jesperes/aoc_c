#include "aoc.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool is_digit(char c) { return c >= '0' && c <= '9'; }

typedef struct {
    int begin_idx;
    int end_idx; // exclusive
    int value;
    bool status;
} substr_t;

substr_t find_number_left(char *str, int start_index) {
    substr_t substr = {0};
    substr.status = false;

    for (int i = start_index; i >= 0; i--) {
        if (is_digit(str[i])) {
            substr.end_idx = i + 1;

            for (int j = i; j >= 0; j--) {
                if (!is_digit(str[j])) {
                    substr.begin_idx = j + 1;
                    char buf[10];
                    int n = i - j;
                    strncpy(buf, &str[j + 1], n);
                    buf[n] = 0;
                    substr.value = atoi(buf);
                    substr.status = true;
                    return substr;
                }
            }
            return substr;
        }
    }
    return substr;
}

substr_t find_number_right(char *str, int start_index) {
    substr_t substr = {0};
    substr.status = false;
    int len = strlen(str);

    for (int i = start_index; i < len; i++) {
        if (is_digit(str[i])) {
            substr.begin_idx = i;
            for (int j = i; j < len; j++) {
                if (!is_digit(str[j])) {
                    substr.end_idx = j;
                    char buf[10];
                    int n = j - i;
                    strncpy(buf, &str[i], n);
                    buf[n] = 0;
                    substr.value = atoi(buf);
                    substr.status = true;
                    return substr;
                }
            }
        }
    }

    return substr;
}

int find_next_idx(char *str, int start_idx, char c) {
    int len = strlen(str);
    for (int i = start_idx; i < len; i++) {
        if (str[i] == c)
            return i;
    }
    assert(0);
}

void snailfish_reduce(char *num) {
    int len = strlen(num);
    int depth = 0;
    for (int i = 0; i < len; i++) {
        if (num[i] == ',')
            continue;

        if (num[i] == '[') {
            if (depth == 4) {
                int left_bracket_idx = i;
                int right_bracket_idx = find_next_idx(num, i, ']');
                // substr_t left = find_number_left(num, i);
                // substr_t right = find_number_right(num, right_bracket_idx);
                num[i] = '0';
                memmove(&num[left_bracket_idx + 1], &num[right_bracket_idx + 1],
                        len - right_bracket_idx);
                return;
            }
            depth++;
            continue;
        } else if (num[i] == ']') {
            depth--;
            continue;
        }

        printf("%d %-*s%c\n", depth, depth * 2, ">", num[i]);
    }
    assert(depth == 0);
}

void day18_selftest() {
    substr_t substr = find_number_left("[[11,22],[33,44]]", 9);
    assert(5 == substr.begin_idx);
    assert(7 == substr.end_idx);
    assert(substr.status);
    assert(substr.value == 22);

    substr = find_number_right("[[11,22],[33,44]]", 4);
    assert(5 == substr.begin_idx);
    assert(7 == substr.end_idx);
    assert(substr.status);
    assert(substr.value == 22);

    char buf[100];
    strcpy(buf, "[[[[[9,8],1],2],3],[2,[3,4]]]");
    printf("buf = %s\n", buf);
    snailfish_reduce(buf);
    printf("buf = %s\n", buf);
}

aoc_result_t day18(const char *input, int len) {
    aoc_result_t result = {0};

    day18_selftest();
    // char *p = input;
    // char *q;
    // while (true) {
    //     char buf[256];

    //     q = strchr(p, '\n');
    //     strncpy(buf, p, (q - p));
    //     snailfish_reduce(buf);
    //     break;
    // }

    return result;
}
