#ifndef __LINKED_QUEUE_H__
#define __LINKED_QUEUE_H__

#include "linked_node.h"

typedef struct {
    linked_node_t* front;
    linked_node_t* rear;
    int length;
} linked_queue_t;

bool linked_queue_init(linked_queue_t* q);

void linked_queue_destory(linked_queue_t* q);

linked_node_t* linked_queue_enq(linked_queue_t* q);

linked_node_t* linked_queue_deq(linked_queue_t* q);

linked_node_t* linked_queue_get(linked_queue_t* q);

#define linked_queue_length(q) ((q)->length)

#define linked_queue_empty(q) ((q)->front == ((void*) 0))

#endif
