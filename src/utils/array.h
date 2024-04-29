#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

#define ARRAY_MIN_CAPACITY 8

#define ARRAY(elem_type)                                                       \
    struct {                                                                   \
        int len;                                                               \
        int capacity;                                                          \
        elem_type *elems;                                                      \
    }

#define ARRAY_INIT_FROM(array, __buf, __len)                                   \
    do {                                                                       \
        (array).elems = __buf;                                                 \
        (array).capacity = __len;                                              \
        (array).len = __len;                                                   \
    } while (0)

// Enlarge the array if necessary. Guarantees that there will be at least room
// to insert one element after calling this.
#define ARRAY_MAYBE_ENLARGE(array)                                             \
    do {                                                                       \
        if ((array).elems == NULL || ((array).len + 1) >= (array).capacity) {  \
            (array).capacity = max(ARRAY_MIN_CAPACITY, (array).capacity * 2);  \
            (array).elems = realloc((array).elems, sizeof((array).elems[0]) *  \
                                                       (array).capacity);      \
        }                                                                      \
    } while (0)

#define ARRAY_ADD(array, elem)                                                 \
    do {                                                                       \
        ARRAY_MAYBE_ENLARGE(array);                                            \
        (array).elems[(array).len++] = elem;                                   \
    } while (0)

#define ARRAY_GET(array, idx) ((array).elems[idx])

#define ARRAY_DEL(array, idx)                                                  \
    do {                                                                       \
        memmove(&(array).elems[idx], &(array).elems[idx + 1],                  \
                sizeof((array).elems[0]) * ((array).len - idx - 1));           \
        (array).len--;                                                         \
    } while (0)

#define ARRAY_LEN(array) ((array).len)

#define ARRAY_FREE(array) (free((array).elems))

// Expands to the type of the array's elements
#define ARRAY_TYPE(array) typeof((array).elems[0])

#define ARRAY_SHRINK(array)                                                    \
    do {                                                                       \
        (array).elems =                                                        \
            realloc((array).elems, (array).len * sizeof((array).elems[0]));    \
        (array).capacity = (array).len;                                        \
    } while (0)

#define ARRAY_ELEMS(array) ((array).elems)

#if 0
void array_selftest() {
    ARRAY(int) foo = {0};
    ARRAY_TYPE(foo) x = 42;
    ARRAY_ADD(foo, x);
    printf("%d\n", ARRAY_GET(foo, 0));
    ARRAY_FREE(foo);
}
#endif