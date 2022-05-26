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

struct node_s* recursion_del(struct node_s* cur, int item) {
    if (cur == NULL) return NULL;

    struct node_s* next = recursion_del(cur->next, item);
    cur->next = next;

    if (cur->data == item) {
        free(cur);
        return next;
    }

    return cur;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 5, 6, 8, 5, 7, 4};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* node = node_dump(arr[0]);
    struct node_s* cur = node;

    for (size_t i = 1; i < arr_len; i++) {
        cur->next = node_dump(arr[i]);
        cur = cur->next;
    }

    struct node_s* ret = recursion_del(node, 5);

    cur = ret;
    while (cur) {
        printf("%d\n", cur->data);
        cur = cur->next;
    }

    return 0;
}
