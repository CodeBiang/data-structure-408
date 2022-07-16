#include "linked_queue.h"

bool linked_queue_init(linked_queue_t* q) {
    q->rear = q->front = NULL;
    q->length = 0;
    return true;
}

void linked_queue_destory(linked_queue_t* q) {
    linked_node_t* node;
    while (!linked_queue_empty(q)) {
        node = linked_queue_deq(q);
        free(node);
    }
}

linked_node_t* linked_queue_enq(linked_queue_t* q) {
    if (q->rear == NULL) {
        q->front = q->rear = (linked_node_t*) malloc(sizeof(linked_node_t));
        return q->front;
    }

    q->rear->next = (linked_node_t*) malloc(sizeof(linked_node_t));
    q->length++;
    q->rear = q->rear->next;
    return q->rear;
}

linked_node_t* linked_queue_deq(linked_queue_t* q) {
    if (!q->front) {
        return NULL;
    }

    linked_node_t* temp = q->front;
    q->length--;
    q->front = q->front->next;
    if (!q->front) {
        q->rear = NULL;
    }
    return temp;
}

linked_node_t* linked_queue_get(linked_queue_t* q) {
    return q->front;
}
