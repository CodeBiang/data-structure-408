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

static struct node_s* intersection_of_ordered_list(struct node_s* la, struct node_s* lb) {
    struct node_s* lc = node_dump(0);
    struct node_s* pcur = lc;

    la = la->next;
    lb = lb->next;

    while (la && lb) {
        if (la->data == lb->data) {
            pcur->next = node_dump(la->data);
            pcur = pcur->next;
            la = la->next;
            lb = lb->next;
        } else if (la->data < lb->data) {
            la = la->next;
        } else {
            lb = lb->next;
        }
    }

    return lc;
}

struct node_s* list_create(int arr[], size_t arr_len) {
    struct node_s* list = node_dump(0);
    struct node_s* cur = list;

    for (size_t i = 0; i < arr_len; i++) {
        cur->next = node_dump(arr[i]);
        cur = cur->next;
    }

    return list;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 5, 5, 6, 7, 8};
    int arr1[] = {1, 5, 5, 5, 6, 7, 9, 11};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = list_create(arr, arr_len);
    struct node_s* lb = list_create(arr1, sizeof(arr1) / sizeof(int));
    struct node_s* cur = intersection_of_ordered_list(l, lb);
    cur = cur->next;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");

    return 0;
}
