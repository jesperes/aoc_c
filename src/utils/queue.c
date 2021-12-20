#include "queue.h"
#include "compare.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void queue_init(queue_t *queue, int size) {
    queue->array = calloc(size, sizeof(int64_t));
    queue->total_size = size;
    queue->head = 0;
    queue->tail = 0;
    queue->num_elems = 0;
}

void queue_deinit(queue_t *queue) { free(queue->array); }

void queue_push(queue_t *queue, int64_t elem) {
    assert(queue->num_elems == (queue->tail - queue->head));
    assert(queue->head + queue->num_elems < queue->total_size);
    queue->array[queue->tail++] = elem;
    queue->num_elems++;
}

int64_t queue_pop(queue_t *queue) {
    int64_t elem = queue->array[queue->head++];
    queue->num_elems--;
    return elem;
}

void queue_sort(queue_t *queue) {
    //   printf("Sorting from index %d + %d elems\n", queue->head,
    //   queue->num_elems),
    qsort(&queue->array[queue->head], queue->num_elems, sizeof(int64_t),
          int64_t_compare_asc);
}

bool queue_is_empty(queue_t *queue) { return queue->num_elems == 0; }

void queue_print(queue_t *queue) {
    printf("<<<\n");
    for (int i = queue->head; i < queue->tail; i++) {
        printf("queue[%d] = %016lx\n", i, queue->array[i]);
    }
    printf(">>>\n");
}
