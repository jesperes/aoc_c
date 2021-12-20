#ifndef COMPARE_H
#define COMPARE_H

#define COMPARE_FUN_HDR(__type)                                                \
    int __type##_compare_asc(const void *p1, const void *p2);                  \
    int __type##_compare_desc(const void *p1, const void *p2);

COMPARE_FUN_HDR(int)
COMPARE_FUN_HDR(int64_t)
COMPARE_FUN_HDR(uint32_t)
COMPARE_FUN_HDR(char)

#endif
