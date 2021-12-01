#include <assert.h>
#include <inttypes.h>
#include <time.h>

#define TS_TO_NS(ts) ((ts).tv_sec * 1000000000 + (ts).tv_nsec)
#define DURATION(start, end) (TS_TO_NS(end) - TS_TO_NS(start))
#define NS_TO_US(ns) ((ns) / 1000.0)

#define REPEAT(Expr, N)                                                        \
    for (int __i = 0; __i < (N); __i++) {                                      \
        Expr;                                                                  \
    }

#define WITH_TIMING(Msg, Expr)                                                 \
    do {                                                                       \
        int reps = 1000;                                                       \
        struct timespec start, end;                                            \
        clock_gettime(CLOCK_REALTIME, &start);                                 \
        REPEAT(Expr, reps);                                                    \
        clock_gettime(CLOCK_REALTIME, &end);                                   \
        printf("%s: %g usecs\n", Msg, NS_TO_US(DURATION(start, end) / reps));  \
    } while (0);

typedef struct {
    int64_t p1, p2;
} aoc_result_t;

aoc_result_t day1();
