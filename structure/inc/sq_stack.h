#ifndef __SQ_STACK_H__
#define __SQ_STACK_H__

#include <stddef.h>
#include <stdbool.h>

/* 顺序表 */
typedef struct sq_stack_s {
    void* data;
    int top;
    size_t item_size;
    size_t capacity;
} sq_stack_t;

bool sq_stack_init(sq_stack_t* l, size_t elem_size, size_t capacity);

void sq_stack_destory(sq_stack_t* l);

void* sq_stack_push(sq_stack_t* l);

void* sq_stack_pop(sq_stack_t* l);

void* sq_stack_top(sq_stack_t* l);

#define sq_stack_empty(l) ((l)->top == -1)
#define sq_stack_overflow(l) ((l)->top == (l)->capacity - 1)

#endif
