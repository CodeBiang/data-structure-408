#include <stdio.h>

#include "sq_stack.h"

#define MAX_SIZE 5
static sq_stack_t s1, s2;

static bool enqueue(int elem) {
    if (sq_stack_overflow(&s1)) {
        if (!sq_stack_empty(&s2)) {
            // overflow
            return false;
        } else {
            // s2 为空 可以逆序复制
            while (!sq_stack_empty(&s1)) {
                *(int*) sq_stack_push(&s2) = *(int*) sq_stack_pop(&s1);
            }
        }
    }

    *(int*) sq_stack_push(&s1) = elem;
    return true;
}

static bool dequeue(int* elem) {
    if (sq_stack_empty(&s2)) {
        // 全空
        if (sq_stack_empty(&s1)) return false;
        // s2 为空 可以逆序复制
        while (!sq_stack_empty(&s1)) {
            *(int*) sq_stack_push(&s2) = *(int*) sq_stack_pop(&s1);
        }
    }

    *elem = *(int*) sq_stack_pop(&s2);
    return true;
}

static bool empty() { return sq_stack_empty(&s1) && sq_stack_empty(&s2); }

int main() {
    sq_stack_init(&s1, sizeof(int), MAX_SIZE);
    sq_stack_init(&s2, sizeof(int), MAX_SIZE);

    for (size_t i = 0; i < 5; i++) {
        enqueue(i);
    }

    while (!empty()) {
        int temp;
        dequeue(&temp);
        printf("%d\n", temp);
    }
    return 0;
}
