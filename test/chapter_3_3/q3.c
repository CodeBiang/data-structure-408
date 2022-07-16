#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_stack.h"

linked_node_t* linked_stack_top_2(linked_stack_t* st) { return st->top ? st->top->next : NULL; }

int func1(int n, int x) {
    switch (n) {
        case 0:
            return 1;
        case 1:
            return 2 * x;
        default:
            return func1(n - 1, x) * 2 * x - 2 * (n - 1) * func1(n - 2, x);
    }
}

int func(int n, int x) {
    linked_stack_t stk;
    linked_stack_init(&stk);
    linked_stack_push(&stk)->data.i = 1;
    linked_stack_push(&stk)->data.i = 2 * x;

    for (int i = 2; i <= n; i++) {
        int pn_1 = linked_stack_top(&stk)->data.i;
        int pn_2 = linked_stack_top_2(&stk)->data.i;
        int ret = 2 * x * pn_1 - 2 * (i - 1) * pn_2;
        linked_stack_push(&stk)->data.i = ret;
    }
    int ret = linked_stack_top(&stk)->data.i;
    linked_stack_destory(&stk);
    return ret;
}

int main() {
    printf("%d\n", func(12, 1));
    printf("%d\n", func1(12, 1));
    return 0;
}
