#include <aoc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define LINES 110
#define BUFSIZE 256

#define IS_OPEN(x) ((x) == '(' || (x) == '[' || (x) == '{' || (x) == '<')
#define IS_CLOSE(x) ((x) == ')' || (x) == ']' || (x) == '}' || (x) == '>')

#define XOR_SWAP(a, b)                                                         \
    do {                                                                       \
        a ^= b;                                                                \
        b ^= a;                                                                \
        a ^= b;                                                                \
    } while (0)

// reverse the given null-terminated string in place, stolen from Stackoverflow
void inplace_reverse(char *str) {
    if (str) {
        char *end = str + strlen(str) - 1;
        while (str < end) {
            XOR_SWAP(*str, *end);
            str++;
            end--;
        }
    }
}

char other(char c) {
    switch (c) {
    case '(':
        return ')';
    case '{':
        return '}';
    case '[':
        return ']';
    case '<':
        return '>';
    default:
        assert(0);
    }
}

bool match_pairs(char *p, int len, char *stack, char **illegal) {
    int top = 0;
    bool is_corrupt = false;
    for (int i = 0; i < len; i++) {
        if (IS_OPEN(p[i])) {
            stack[top++] = other(p[i]);
        } else if (IS_CLOSE(p[i])) {
            if (stack[top - 1] == p[i]) {
                top--;
            } else {
                is_corrupt = true;
                *illegal = &p[i];
                break;
            }
        }
    }
    stack[top] = 0;
    return is_corrupt;
}

int syntax_score(char c) {
    switch (c) {
    case ')':
        return 3;
    case ']':
        return 57;
    case '}':
        return 1197;
    case '>':
        return 25137;
    default:
        assert(0);
        break;
    }
}

int64_t completion_value(char c) {
    switch (c) {
    case ')':
        return 1;
    case ']':
        return 2;
    case '}':
        return 3;
    case '>':
        return 4;
    default:
        assert(0);
        break;
    }
}

int64_t score_completion(char *s) {
    int len = strlen(s);
    int64_t score = 0;
    for (int i = 0; i < len; i++) {
        score = score * 5 + completion_value(s[i]);
    }
    return score;
}

int int64_compare(const void *p1, const void *p2) {
    int64_t x = *(int64_t *)p1;
    int64_t y = *(int64_t *)p2;
    return (x > y) - (x < y);
}

aoc_result_t day10(char *ptr, int len) {
    aoc_result_t result = {0};
    char stack[BUFSIZE];
    char *p = ptr;
    int64_t p2scores[200];
    int currp2 = 0;

    int line = 0;
    while (true) {
        char *q = strchr(p, '\n');
        int l = q - p;
        char *illegal;

        if (match_pairs(p, l, stack, &illegal)) {
            result.p1 += syntax_score(*illegal);
        } else {
            inplace_reverse(stack);
            int64_t score = score_completion(stack);
            p2scores[currp2++] = score;
        }
        line++;
        q++;
        if (q - ptr >= len)
            break;
        p = q;
    }

    qsort(p2scores, currp2, sizeof(int64_t), int64_compare);

    int middle = (currp2 / 2);
    result.p2 = p2scores[middle];

    return result;
}
