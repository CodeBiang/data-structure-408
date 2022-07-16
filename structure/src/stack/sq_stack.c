#include "sq_stack.h"

#include <stdlib.h>

bool sq_stack_init(sq_stack_t* st, size_t elem_size, size_t capacity) {
    st->item_size = elem_size;
    st->capacity = capacity;
    st->top = -1;
    st->data = malloc(elem_size * capacity);
    return st->data != NULL;
}

void sq_stack_destory(sq_stack_t* st) {
    if (st->data)
        // free
        free(st->data);
}

void* sq_stack_push(sq_stack_t* st) {
    void* ret;
    if (st->top + 1 < (int) st->capacity) {
        ret = (void*) ((char*) st->data + st->item_size * (++st->top));
        return ret;
    }
    return NULL;
}

void* sq_stack_pop(sq_stack_t* st) {
    return st->top != -1 ? (void*) ((char*) st->data + st->item_size * (st->top--)) : NULL;
}

void* sq_stack_top(sq_stack_t* st) {
    return st->top != -1 ? (void*) ((char*) st->data + st->item_size * st->top) : NULL;
}
