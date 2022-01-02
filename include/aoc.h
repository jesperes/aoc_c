#ifndef _AOC_H
#define _AOC_H

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <time.h>

#define UNUSED __attribute__((unused))

#define TS_TO_NS(ts) ((ts).tv_sec * 1000000000 + (ts).tv_nsec)
#define DURATION(start, end) (TS_TO_NS(end) - TS_TO_NS(start))
#define NS_TO_US(ns) ((ns) / 1000.0)

// TODO handle non-integer results
typedef struct {
    int64_t p1, p2;
} aoc_result_t;

void assert_int64(const char *msg, int64_t actual, int64_t expected);
void assert_string(const char *msg, const char *actual, const char *expected);

#define PRINTRES(result) printf("p1 = %ld, p2 = %ld\n", result.p1, result.p2);

#endif
