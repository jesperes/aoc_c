#include "aoc2021.h"
#include <inttypes.h>

int main() {
    int64_t total_time = 0;

    RUN_PUZZLE("day1", day1, 2021, 1, aoc_result_t, 1400, 1429, total_time);
    RUN_PUZZLE("day2", day2, 2021, 2, aoc_result_t, 1962940, 1813664422,
               total_time);
    RUN_PUZZLE("day3", day3, 2021, 3, aoc_result_t, 3549854, 3765399,
               total_time);
    RUN_PUZZLE("day4", day4, 2021, 4, aoc_result_t, 33462, 30070, total_time);

    printf("Total: %g msecs\n", total_time / 1000000.0);
}
