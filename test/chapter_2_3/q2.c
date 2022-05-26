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

static void remove_all(struct node_s* list, int item) {
    struct node_s* prev = list;
    struct node_s* cur = list->next;

    while (cur) {
        if (cur->data == item) {
            prev->next = cur->next;
            free(cur);
            cur = prev->next;
        } else {
            prev = cur;
            cur = cur->next;
        }
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 5, 6, 8, 5, 7, 4};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* list = node_dump(0);
    struct node_s* cur = list;

    for (size_t i = 0; i < arr_len; i++) {
        cur->next = node_dump(arr[i]);
        cur = cur->next;
    }

    remove_all(list, 5);

    cur = list->next;
    while (cur) {
        printf("%d\n", cur->data);
        cur = cur->next;
    }

    return 0;
}
