#ifndef __SQ_QUEUE_H__
#define __SQ_QUEUE_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct sq_queue_s sq_queue_t;

struct sq_queue_s {
    void* data;
    int front, rear;
    size_t item_size;
    size_t capacity;
};

bool sq_queue_init(sq_queue_t* q, size_t elem_size, size_t capacity);

void sq_queue_destory(sq_queue_t* q);

void* sq_queue_enq(sq_queue_t* q);

void* sq_queue_deq(sq_queue_t* q);

void* sq_queue_get(sq_queue_t* q);

int sq_queue_length(sq_queue_t* q);

#define sq_queue_empty(q) ((q)->front == (q)->rear) 

#endif
