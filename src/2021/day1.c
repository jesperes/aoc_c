#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void day1() {
    //
    FILE *f = fopen("inputs/2021/input01.txt", "rb");
    assert(f != NULL);
    char *linebuf = malloc(10);
    size_t n;
    ssize_t bytes;
    int a0 = INT_MAX, a1 = 0;
    int i = 0;
    int win[4] = {0};
    int num_increases1 = 0, num_increases2 = 0;

    while ((bytes = getline(&linebuf, &n, f)) != -1) {
        linebuf[bytes - 1] = '\0'; // trim newline
        a1 = win[i % 4] = atoi(linebuf);
        if (a1 > a0) {
            num_increases1++;
        }
        a0 = a1;

        if (i < 3) {
            i++;
            continue;
        }

        int w1 = win[(i + 1) % 4];
        int w2 = win[(i + 2) % 4];
        int w3 = win[(i + 3) % 4];
        int w4 = win[i % 4];

        if (w1 + w2 + w3 < w2 + w3 + w4) {
            num_increases2++;
        }

        i++;
    }

    printf("%d\n", num_increases1);
    printf("%d\n", num_increases2);
    free(linebuf);
}
