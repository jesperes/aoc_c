#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int64_t *array;
    int total_size;
    int num_elems;
    int head;
    int tail;
} queue_t;

void queue_init(queue_t *queue, int size);
void queue_deinit(queue_t *queue);
void queue_push(queue_t *queue, int64_t elem);
int64_t queue_pop(queue_t *queue);
void queue_sort(queue_t *queue);
bool queue_is_empty(queue_t *queue);
void queue_print(queue_t *queue);