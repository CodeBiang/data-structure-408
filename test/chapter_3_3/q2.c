#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sq_stack.h"

int main() {
    sq_stack_t origin;
    sq_stack_init(&origin, sizeof(char*), 10);
    *(char**) sq_stack_push(&origin) = "H1";
    *(char**) sq_stack_push(&origin) = "H2";
    *(char**) sq_stack_push(&origin) = "S1";
    *(char**) sq_stack_push(&origin) = "S2";
    *(char**) sq_stack_push(&origin) = "H3";
    *(char**) sq_stack_push(&origin) = "S4";
    *(char**) sq_stack_push(&origin) = "H4";

    sq_stack_t hstack;
    sq_stack_init(&hstack, sizeof(char*), 10);
    while (!sq_stack_empty(&origin)) {
        char* s = *(char**) sq_stack_pop(&origin);
        if (s[0] == 'S') {
            printf("%s ", s);
        } else
            *(char**) sq_stack_push(&hstack) = s;
    }
    while (!sq_stack_empty(&hstack)) {
        char* s = *(char**) sq_stack_pop(&hstack);
        printf("%s ", s);
    }
    printf("\n");
    return 0;
}
