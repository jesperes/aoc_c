#include "utils.h"
#include <stdlib.h>
#include <string.h>

#define MAXLINES 1024

char **split_input_to_lines(char *input, int len, int *numlines) {
    char **lines = malloc(MAXLINES * sizeof(char *));
    char *p = input;
    int i = 0;
    while (p - input < len) {
        char *q = strchr(p, '\n');
        if (q == NULL) {
            lines[i++] = strndup(p, len - (p - input));
            break;
        } else {
            lines[i++] = strndup(p, (q - p));
            p = q + 1;
        }
    }
    *numlines = i;
    return lines;
}

void free_lines(char **lines, int numlines) {
    for (int i = 0; i < numlines; i++) {
        free(lines[i]);
    }
    free(lines);
}