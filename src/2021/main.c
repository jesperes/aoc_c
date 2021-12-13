#include "aoc.h"
#include "aoc2021.h"
#include <inttypes.h>
#include <time.h>

int main() {
    int64_t total_time = 0;
    int days = 5;

    RUN_PUZZLE("day1", day1, 2021, 1, aoc_result_t, 1400, 1429, total_time);
    RUN_PUZZLE("day2", day2, 2021, 2, aoc_result_t, 1962940, 1813664422,
               total_time);
    RUN_PUZZLE("day3", day3, 2021, 3, aoc_result_t, 3549854, 3765399,
               total_time);
    RUN_PUZZLE("day4", day4, 2021, 4, aoc_result_t, 33462, 30070, total_time);

    RUN_PUZZLE("day5", day5, 2021, 5, aoc_result_t, 4728, 17717, total_time);
    RUN_PUZZLE("day6", day6, 2021, 6, aoc_result_t, 375482, 1689540415957,
               total_time);

    RUN_PUZZLE_NOINPUT("day7", day7, aoc_result_t, 356179, 99788435,
                       total_time);

    RUN_PUZZLE("day8", day8, 2021, 8, aoc_result_t, 548, 1074888, total_time);
    RUN_PUZZLE("day9", day9, 2021, 9, aoc_result_t, 524, 1235430, total_time);
    RUN_PUZZLE("day10", day10, 2021, 10, aoc_result_t, 392043, 1605968119,
               total_time);
    RUN_PUZZLE("day11", day11, 2021, 11, aoc_result_t, 1675, 515, total_time);
    RUN_PUZZLE("day12", day12, 2021, 12, aoc_result_t, 4549, 120535,
               total_time);

    printf("Total: %g msecs\n", total_time / 1000000.0);

    int64_t avg_per_day = total_time / days;
    int64_t predicted_total = avg_per_day * 25;

    printf("Average ms/day: %g ms\n", (total_time / days) / 1000000.0);
    printf("Predicted total: %g ms\n",
           predicted_total / 1000000.0);
}
