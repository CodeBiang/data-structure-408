#include <stdio.h>
#include <stdlib.h>

#include "linked_node.h"

struct {
    linked_node_t* front;
    linked_node_t* rear;
} queue;

#define queue_empty() (queue.front == queue.rear)

linked_node_t* enqueue() {
    linked_node_t* node;
    
    // 队未满直接利用已有空间
    if (queue.rear && queue.rear->next != queue.front) {
        node = queue.rear->next;
        queue.rear = node;
        return node;
    }
    
    // 队满/队空需要申请空间
    node = malloc(sizeof(linked_node_t));
    printf("malloc\n");
    if (!queue.front) {
        queue.front = malloc(sizeof(linked_node_t));
        queue.front->next = node;
        queue.rear = node;
        node->next = queue.front;
        return node;
    }

    node->next = queue.rear->next;
    queue.rear->next = node; 
    queue.rear = node;
    return node;
}

// 因为多出一个节点，要保证出队的是front.next
linked_node_t* dequeue() {
    if (queue_empty()) {
        // 空
        return NULL;    
    }
    linked_node_t* node = queue.front->next;
    queue.front = queue.front->next;
    
    return node;
}


int main() {
    enqueue()->data.i = 1;
    enqueue()->data.i = 2;
    enqueue()->data.i = 3;
    enqueue()->data.i = 4;
    enqueue()->data.i = 5;
    enqueue()->data.i = 6;

    while (!queue_empty()) {
        printf("%d\n", dequeue()->data.i);
    }
    enqueue()->data.i = 1;
    enqueue()->data.i = 2;
    while (!queue_empty()) {
        printf("%d\n", dequeue()->data.i);
    }
    return 0;
}
