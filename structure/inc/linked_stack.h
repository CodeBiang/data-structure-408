#ifndef __LINKED_STACK_H__
#define __LINKED_STACK_H__

#include "linked_node.h"

typedef struct {
    linked_node_t* top;
} linked_stack_t;

bool linked_stack_init(linked_stack_t* st);

void linked_stack_destory(linked_stack_t* st);

linked_node_t* linked_stack_push(linked_stack_t* st);

linked_node_t* linked_stack_pop(linked_stack_t* st);

linked_node_t* linked_stack_top(linked_stack_t* st);

#define linked_stack_empty(st) ((st)->top == ((void*) 0))

#endif
