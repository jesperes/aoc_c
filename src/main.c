#include "aoc2021.h"
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

int main() {
    WITH_TIMING("day1", {
        aoc_result_t res = day1();
        assert(res.p1 == 1400);
        assert(res.p2 == 1429);
    });
}
