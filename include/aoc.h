#ifndef _AOC_H
#define _AOC_H

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <time.h>

#ifndef NUM_REPS
#define NUM_REPS 100
#endif

#define UNUSED __attribute__((unused))

#define TS_TO_NS(ts) ((ts).tv_sec * 1000000000 + (ts).tv_nsec)
#define DURATION(start, end) (TS_TO_NS(end) - TS_TO_NS(start))
#define NS_TO_US(ns) ((ns) / 1000.0)

#define REPEAT(Expr, N)                                                        \
    for (int __i = 0; __i < (N); __i++) {                                      \
        Expr;                                                                  \
    }

#define WITH_TIMING(Msg, TotalTime, Expr)                                      \
    do {                                                                       \
        int reps = NUM_REPS;                                                   \
        struct timespec start, end;                                            \
        clock_gettime(CLOCK_REALTIME, &start);                                 \
        REPEAT(Expr, reps);                                                    \
        clock_gettime(CLOCK_REALTIME, &end);                                   \
        int64_t duration = DURATION(start, end) / reps;                        \
        (TotalTime) += duration;                                               \
        printf("%-6s %10.2f usecs\n", Msg, duration / 1000.0);                 \
    } while (0);

#define RUN_PUZZLE(Msg, Function, Year, Day, ResultType, P1, P2, AssertFun1,   \
                   AssertFun2, TotalTime)                                      \
    do {                                                                       \
        extern unsigned char input##Day##_txt[];                               \
        extern unsigned int input##Day##_txt_len;                              \
        char *ptr = (char *)input##Day##_txt;                                  \
        unsigned int len = input##Day##_txt_len;                               \
        WITH_TIMING(Msg, TotalTime, {                                          \
            ResultType __result = Function(ptr, len);                          \
            AssertFun1(Msg " p1", __result.p1, P1);                            \
            AssertFun2(Msg " p2", __result.p2, P2);                            \
        });                                                                    \
    } while (0);

#define RUN_PUZZLE_INT64(Msg, Function, Year, Day, ResultType, P1, P2,         \
                         TotalTime)                                            \
    RUN_PUZZLE(Msg, Function, Year, Day, ResultType, P1, P2, assert_int64,     \
               assert_int64, TotalTime)

#define RUN_PUZZLE_NOINPUT_INT64(Msg, Function, ResultType, P1, P2, TotalTime) \
    do {                                                                       \
        WITH_TIMING(Msg, TotalTime, {                                          \
            ResultType __result = Function();                                  \
            assert_int64(Msg " p1", __result.p1, P1);                          \
            assert_int64(Msg " p2", __result.p2, P2);                          \
        });                                                                    \
    } while (0);

// TODO handle non-integer results
typedef struct {
    int64_t p1, p2;
} aoc_result_t;

typedef struct {
    int64_t p1;
    char p2[32];
} aoc_result_int_string_t;

void assert_int64(const char *msg, int64_t actual, int64_t expected);
void assert_string(const char *msg, const char *actual, const char *expected);

#define PRINTRES(result) printf("p1 = %ld, p2 = %ld\n", result.p1, result.p2);

#endif
