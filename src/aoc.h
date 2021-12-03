#include "mmap_input.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <time.h>

#define UNUSED __attribute__((unused))

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
        printf("%s: %g usecs\n", Msg,                                          \
               ((double)NS_TO_US(DURATION(start, end)) / reps));               \
    } while (0);

#define RUN_PUZZLE(Msg, Function, Year, Day, ResultType, P1, P2)               \
    do {                                                                       \
        extern unsigned char inputs_##Year##_input##Day##_txt[];               \
        extern unsigned int inputs_##Year##_input##Day##_txt_len;              \
        char *ptr = (char *)inputs_##Year##_input##Day##_txt;                  \
        unsigned int len = inputs_##Year##_input##Day##_txt_len;               \
        WITH_TIMING(Msg, {                                                     \
            ResultType __result = Function(ptr, len);                          \
            assert(__result.p1 == P1);                                         \
            assert(__result.p2 == P2);                                         \
        });                                                                    \
    } while (0);

// TODO handle non-integer results
typedef struct {
    int64_t p1, p2;
} aoc_result_t;
