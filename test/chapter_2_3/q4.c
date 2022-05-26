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

static void remove_min(struct node_s* list) {
    struct node_s* cur = list->next;
    struct node_s* prev = list;

    struct node_s* min = cur;
    struct node_s* min_prev = prev;

    while (cur) {
        if (cur->data < min->data) {
            min = cur;
            min_prev = prev;
        }
        prev = cur;
        cur = cur->next;
    }

    min_prev->next = min->next;
    free(min);
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 0, 5, 6, 8, 5, 7, 4};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* list = node_dump(0);
    struct node_s* cur = list;

    for (size_t i = 0; i < arr_len; i++) {
        cur->next = node_dump(arr[i]);
        cur = cur->next;
    }

    remove_min(list);

    cur = list->next;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
    return 0;
}
