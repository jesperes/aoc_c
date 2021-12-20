#include <stdint.h>

#define COMPARE_FUN(__type)                                                    \
    int __type##_compare_asc(const void *__p1, const void *__p2) {             \
        __type x = *(__type *)__p1;                                            \
        __type y = *(__type *)__p2;                                            \
        if (x < y)                                                             \
            return -1;                                                         \
        else if (x > y)                                                        \
            return 1;                                                          \
        else                                                                   \
            return 0;                                                          \
    }                                                                          \
                                                                               \
    int __type##_compare_desc(const void *__p1, const void *__p2) {            \
        __type x = *(__type *)__p1;                                            \
        __type y = *(__type *)__p2;                                            \
        if (y < x)                                                             \
            return -1;                                                         \
        else if (y > x)                                                        \
            return 1;                                                          \
        else                                                                   \
            return 0;                                                          \
    }

COMPARE_FUN(int)
COMPARE_FUN(int64_t)
COMPARE_FUN(uint32_t)
COMPARE_FUN(char)
