#include "sq_queue.h"

#include <stdlib.h>

bool sq_queue_init(sq_queue_t* q, size_t elem_size, size_t capacity) {
    q->item_size = elem_size;
    // 牺牲一个单元, rear + 1 取模 和 front 相等可作为队满标志
    q->capacity = capacity + 1;
    q->data = malloc(elem_size * capacity);
    q->front = q->rear = 0;
    return true;
}

void sq_queue_destory(sq_queue_t* q) {
    if (q->data)
        // free
        free(q->data);
}

void* sq_queue_enq(sq_queue_t* q) {
    int newrear = (q->rear + 1) % q->capacity;
    if (newrear == q->front) return NULL;

    void* ret = (void*) ((char*) q->data + q->item_size * (q->rear));
    q->rear = newrear;
    return ret;
}

void* sq_queue_deq(sq_queue_t* q) {
    if (q->rear == q->front) return NULL;
    int newfront = (q->front + 1) % q->capacity;

    void* ret = (void*) ((char*) q->data + q->item_size * (q->front));
    q->front = newfront;
    return ret;
}

void* sq_queue_get(sq_queue_t* q) {
    return (void*) ((char*) q->data + q->item_size * (q->front));
}

int sq_queue_length(sq_queue_t* q) { return (q->rear - q->front + q->capacity) % q->capacity; }
