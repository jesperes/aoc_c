#include "aoc.h"
#include "aoc2021.h"
#include <inttypes.h>

int main() {
    int64_t total_time = 0;
    int days = 5;

    // Aim for a total runtime of 50ms.
    int64_t budget = 50 * 1000000;

    RUN_PUZZLE("day1", day1, 2021, 1, aoc_result_t, 1400, 1429, total_time);
    RUN_PUZZLE("day2", day2, 2021, 2, aoc_result_t, 1962940, 1813664422,
               total_time);
    RUN_PUZZLE("day3", day3, 2021, 3, aoc_result_t, 3549854, 3765399,
               total_time);
    RUN_PUZZLE("day4", day4, 2021, 4, aoc_result_t, 33462, 30070, total_time);

    RUN_PUZZLE("day5", day5, 2021, 5, aoc_result_t, 4728, 17717, total_time);

    printf("Total: %g msecs\n", total_time / 1000000.0);
    printf("Budget (%ld ms) spent: %ld%%\n", budget / 1000000,
           100 * total_time / budget);

    int64_t avg_per_day = total_time / days;
    int64_t predicted_total = avg_per_day * 25;

    printf("Average ms/day: %g ms\n", (total_time / days) / 1000000.0);
    printf("Predicted total: %g ms (%ld%% of budget)\n",
           predicted_total / 1000000.0, 100 * predicted_total / budget);
}
