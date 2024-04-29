#include "aoc.h"
#include "aoc2021.h"
#include "array.h"
#include "hashtable.h"
#include <dlfcn.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Keep VS code happy, for some reason
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 1
#endif

typedef aoc_result_t (*puzzle_fun_t)(char *input, int len);

typedef struct {
    int nr;
    puzzle_fun_t fun;
    aoc_result_t expected;
    char *input;
    int len;
} day_t;

char *get_input_for_day(int nr) {
    char buf[32];
    sprintf(buf, "input%d_txt", nr);
    void *sym = dlsym(NULL, buf);
    if (sym == NULL)
        return NULL;
    else
        return (char *)sym;
}

int get_input_len_for_day(int nr) {
    char buf[32];
    sprintf(buf, "input%d_txt_len", nr);
    void *sym = dlsym(NULL, buf);
    if (sym == NULL)
        return 0;
    else
        return *(int *)sym;
}

puzzle_fun_t get_fun_for_day(int nr) {
    char buf[32];
    sprintf(buf, "day%d", nr);
    void *sym = dlsym(NULL, buf);
    assert(sym != NULL);
    return (puzzle_fun_t)sym;
}

day_t day_i64(int nr, int64_t p1, int64_t p2) {
    day_t day;
    day.nr = nr;
    day.expected.p1 = p1;
    day.expected.p2 = p2;
    day.fun = get_fun_for_day(nr);
    day.input = get_input_for_day(nr);
    day.len = get_input_len_for_day(nr);
    return day;
}

#define TS_TO_NS(ts) ((ts).tv_sec * 1000000000 + (ts).tv_nsec)
#define DURATION(start, end) (TS_TO_NS(end) - TS_TO_NS(start))
#define NS_TO_US(ns) ((ns) / 1000.0)

enum Mode { Normal, Benchmark };

void run(enum Mode mode, day_t day) {
    char buf[32];
    if (day.fun == NULL) {
        printf("-- not implemented --\n");
        return;
    } else {
        switch (mode) {
        case Normal: {
            sprintf(buf, "Day %d", day.nr);
            printf("Day %2d ", day.nr);
            aoc_result_t result = (*day.fun)(day.input, day.len);
            assert_int64(buf, result.p1, day.expected.p1);
            assert_int64(buf, result.p2, day.expected.p2);
            printf("ok: %ld/%ld\n", day.expected.p1, day.expected.p2);
            break;
        }
        case Benchmark: {
            char *spinner[] = {"↑", "↗", "→", "↘", "↓", "↙", "←", "↖"};
            int spinlen = 8;
            int spincount = 0;
            int maxreps = 100;
            int64_t duration_ns = 0;
            int64_t secs = 5;

            struct timespec progress;
            clock_gettime(CLOCK_REALTIME, &progress);
            int reps = 0;
            while (reps < maxreps) {
                reps++;
                struct timespec start;
                clock_gettime(CLOCK_REALTIME, &start);

                aoc_result_t result = (*day.fun)(day.input, day.len);

                struct timespec now;
                clock_gettime(CLOCK_REALTIME, &now);

                assert_int64(buf, result.p1, day.expected.p1);
                assert_int64(buf, result.p2, day.expected.p2);

                duration_ns += DURATION(start, now);
                if (duration_ns > (secs * 1000000000))
                    break;

                if (DURATION(progress, now) > 125 * 1000 * 1000) {
                    printf("\rDay %2d: %s  ", day.nr,
                           spinner[spincount++ % spinlen]);
                    fflush(stdout);
                    clock_gettime(CLOCK_REALTIME, &progress);
                }
            }

            struct timespec end;
            clock_gettime(CLOCK_REALTIME, &end);
            int64_t ns_per_rep = duration_ns / reps;
            printf("\rDay %2d: %8ld \u00B5s\n", day.nr, ns_per_rep / 1000);
            break;
        }
        }
    }
}

int main(int argc, char **argv) {
    enum Mode mode = Normal;
    int day = -1;
    day_t days[25] = {0};
    int i = 0;

    days[i++] = day_i64(1, 1400, 1429);
    days[i++] = day_i64(2, 1962940, 1813664422);
    days[i++] = day_i64(3, 3549854, 3765399);
    days[i++] = day_i64(4, 33462, 30070);
    days[i++] = day_i64(5, 4728, 17717);
    days[i++] = day_i64(6, 375482, 1689540415957);
    days[i++] = day_i64(7, 356179, 99788435);
    days[i++] = day_i64(8, 548, 1074888);
    days[i++] = day_i64(9, 524, 1235430);
    days[i++] = day_i64(10, 392043, 1605968119);
    days[i++] = day_i64(11, 1675, 515);
    days[i++] = day_i64(12, 4549, 120535);
    days[i++] = day_i64(13, 701, 0);
    days[i++] = day_i64(14, 3831, 5725739914282);
    days[i++] = day_i64(15, 386, 2806);
    days[i++] = day_i64(16, 927, 1725277876501);
    days[i++] = day_i64(17, 30628, 4433);
    days[i++] = day_i64(18, 3486, 4747);
    days[i++] = day_i64(19, 0, 0);
    days[24] = day_i64(25, 295, 0);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0) {
            mode = Benchmark;
            continue;
        } else {
            char *p;
            day = strtol(argv[i], &p, 10);
            if (p == argv[i]) {
                printf("Invalid arg: %s\n", argv[i]);
                assert(0);
            }
        }
    }

    if (mode == Benchmark) {
        printf("Benchmark mode.\n");
    }

    if (day != -1) {
        run(mode, days[day - 1]);
    } else {
        for (int i = 0; i < 25; i++) {
            run(mode, days[i]);
        }
    }
}
