#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_node.h"

#define YES 1
#define DONE 0
#define NO -1
#define TAIL -2

int is_node_same(linked_node_t** n1, const linked_node_t* n2) {
    if (*n1 == NULL) return NO;
    if (n2 == NULL) return TAIL;
    int ret = is_node_same(n1, n2->next);
    switch (ret) {
        case YES:
        case TAIL:
            if (*n1 == n2) return DONE;
            if ((*n1)->data.c == n2->data.c) {
                *n1 = (*n1)->next;
                return (*n1) == n2 ? DONE : YES;
            } else
                return NO;
        default:
            return ret;
    }
}

bool is_symmetry(linked_node_t* node) { return is_node_same(&node, node->next) == DONE; }

int main() {
    linked_node_t* n = linked_node_create();
    n->data.c = 'x';
    n->next = NULL;
    n = linked_node_link_create(n);
    n->data.c = 'y';
    n = linked_node_link_create(n);
    n->data.c = 'y';
    n = linked_node_link_create(n);
    n->data.c = 'x';

    printf("%d\n", is_symmetry(n));
    return 0;
}
