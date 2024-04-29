#include "utils.h"
#include "array.h"
#include <stdlib.h>
#include <string.h>
#define MAXLINES 2048

char **split_input_to_lines(char *input, int len, int *numlines) {
    ARRAY(char *) lines = {0};
    char *p = input;
    while (p - input < len) {
        char *q = strchr(p, '\n');
        if (q == NULL) {
            ARRAY_ADD(lines, strndup(p, len - (p - input)));
            break;
        } else {
            ARRAY_ADD(lines, strndup(p, (q - p)));
            p = q + 1;
        }
    }
    *numlines = ARRAY_LEN(lines);
    ARRAY_SHRINK(lines);
    return ARRAY_ELEMS(lines);
}

void free_lines(char **array, int numlines) {
    ARRAY(char *) lines = {0};
    ARRAY_INIT_FROM(lines, array, numlines);
    for (int i = 0; i < ARRAY_LEN(lines); i++) {
        free(ARRAY_GET(lines, i));
    }
    ARRAY_FREE(lines);
}