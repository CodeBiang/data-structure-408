#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_stack.h"

static bool brackets_judge(const char* s, size_t len) {
    static const char brackets[3] = "([{";
    bool ret;
    linked_stack_t brackets_stack;
    linked_node_t* node;
    int idx;
    linked_stack_init(&brackets_stack);

    for (size_t i = 0; i < len; i++) {
        idx = -1;
        switch (s[i]) {
            case '(':
            case '[':
            case '{':
                linked_stack_push(&brackets_stack)->data.c = s[i];
                break;
            case ')':
                node = linked_stack_pop(&brackets_stack);
                idx = 0;
                break;
            case ']':
                node = linked_stack_pop(&brackets_stack);
                idx = 1;
                break;
            case '}':
                node = linked_stack_pop(&brackets_stack);
                idx = 2;
                break;
        }

        if (idx != -1) {
            if (!node || node->data.c != brackets[idx]) {
                linked_stack_destory(&brackets_stack);
                return false;
            }
            free(node);
        }
    }

    ret = linked_stack_empty(&brackets_stack);
    linked_stack_destory(&brackets_stack);

    return ret;
}

int main() {
    const char* expression = "(fdsaf(fasdf)FAsfasd)";

    printf("%s\n", brackets_judge(expression, strlen(expression)) ? "true" : "false");

    return 0;
}
