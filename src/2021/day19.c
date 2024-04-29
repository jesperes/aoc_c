#include "aoc.h"
#include "array.h"
#include "utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BEACONS 32
#define NUM_SCANNERS 50
#define NUM_ROTATIONS 24

typedef struct {
    int x;
    int y;
    int z;
} coord_t;

typedef struct {
    int id;
    ARRAY(coord_t) coords;
} scanner_t;

coord_t dist_vector(coord_t a, coord_t b) {
    coord_t dv = {a.x - b.x, a.y - b.y, a.z - b.z};
    return dv;
}

int manhattan_dist(coord_t a, coord_t b) {
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}

inline coord_t coord(int x, int y, int z) {
    coord_t c = {x, y, z};
    return c;
}

#define ROTATE(c, x0, y0, z0)                                                  \
    {                                                                          \
        (c).x = x0;                                                            \
        (c).y = y0;                                                            \
        (c).z = z0;                                                            \
    }

void rotate(coord_t a, coord_t r[NUM_ROTATIONS]) {
    int x = a.x;
    int y = a.y;
    int z = a.z;

    ROTATE(r[0], x, y, z);
    ROTATE(r[1], x, -y, -z);
    ROTATE(r[2], x, z, -y);
    ROTATE(r[3], x, -z, y);
    ROTATE(r[4], -z, y, -z);

    ROTATE(r[5], -x, -y, z);
    ROTATE(r[6], -x, z, y);
    ROTATE(r[7], -x, -z, -y);
    ROTATE(r[8], y, x, -z);
    ROTATE(r[9], y, -x, z);

    ROTATE(r[10], y, z, x);
    ROTATE(r[11], y, -z, -x);
    ROTATE(r[12], -y, x, z);
    ROTATE(r[13], -y, -x, -z);
    ROTATE(r[14], -y, z, -x);

    ROTATE(r[15], -y, -z, x);
    ROTATE(r[16], z, x, y);
    ROTATE(r[17], z, -x, -y);
    ROTATE(r[18], z, y, -x);
    ROTATE(r[19], z, -y, x);

    ROTATE(r[20], -z, x, -y);
    ROTATE(r[21], -z, -x, y);
    ROTATE(r[22], -z, y, x);
    ROTATE(r[23], -z, -y, -x);
}

aoc_result_t day19(char *input, int len) {
    aoc_result_t result = {0};
    ARRAY(scanner_t) scanners = {0};
    int numlines;
    char **lines = split_input_to_lines(input, len, &numlines);

    for (int i = 0; i < numlines; i++) {
        int id = 0;
        int x, y, z;

        if (sscanf(lines[i], "--- scanner %d ---", &id) == 1) {
            scanner_t scanner = {0};
            scanner.id = id;
            ARRAY_ADD(scanners, scanner);
        } else if (sscanf(lines[i], "%d,%d,%d", &x, &y, &z) == 3) {
            scanner_t *scanner = &ARRAY_GET(scanners, ARRAY_LEN(scanners) - 1);
            coord_t coord = {x, y, z};
            ARRAY_ADD(scanner->coords, coord);
        } else if (strlen(lines[i]) == 0) {
            continue;
        } else {
            assert(0);
        }
    }

#if 0
    for (int i = 0; i < num_scanners; i++) {
        printf("Scanner %d\n", scanners[i].id);
        for (int j = 0; j < scanners[i].num_beacons; j++) {
            printf("  Beacon %d: (%d,%d,%d)\n", j, scanners[i].beacons[j].x,
                   scanners[i].beacons[j].y, scanners[i].beacons[j].z);
        }
    }
#endif

    return result;
}