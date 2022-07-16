#include "linked_stack.h"

#include <stdlib.h>

bool linked_stack_init(linked_stack_t* st) { st->top = NULL; }

void linked_stack_destory(linked_stack_t* st) {
    linked_node_t *cur, *temp;
    cur = st->top;

    while (cur) {
        temp = cur->next;
        free(cur);
        cur = temp;
    }
}

linked_node_t* linked_stack_push(linked_stack_t* st) {
    linked_node_t* node = (linked_node_t*) malloc(sizeof(linked_node_t));

    node->next = st->top;
    st->top = node;

    return node;
}

linked_node_t* linked_stack_pop(linked_stack_t* st) {
    linked_node_t* node = st->top;
    if (!node) return node;
    st->top = st->top->next;
    return node;
}

linked_node_t* linked_stack_top(linked_stack_t* st) { return st->top; }
