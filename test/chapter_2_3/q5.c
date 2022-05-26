#include <stdio.h>
#include <stdlib.h>

struct node_s {
    int data;
    struct node_s* next;
};

static inline struct node_s* node_dump(int value) {
    struct node_s* ret = (struct node_s*) malloc(sizeof(struct node_s));
    ret->data = value;
    ret->next = NULL;
    return ret;
}

static inline void node_reverse(struct node_s* node, struct node_s** new_head) {
    if (!node->next) {
        *new_head = node;
        return;
    }
    node_reverse(node->next, new_head);
    node->next->next = node;
    node->next = NULL;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* list = node_dump(0);
    struct node_s* cur = list;

    for (size_t i = 0; i < arr_len; i++) {
        cur->next = node_dump(arr[i]);
        cur = cur->next;
    }

    node_reverse(list->next, &list->next);

    cur = list->next;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
    return 0;
}
