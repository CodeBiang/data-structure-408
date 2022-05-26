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

static void intersection_of_ordered_list(struct node_s** pla, struct node_s* lb) {
    struct node_s* pre = NULL;
    struct node_s* temp;
    struct node_s* la = *pla;

    while (la && lb) {
        if (la->data == lb->data) {
            pre = la;
            la = la->next;
            lb = lb->next;
        } else if (la->data < lb->data) {
            if (pre) {
                pre->next = la->next;
            } else {
                *pla = la->next;
            }
            temp = la;
            la = la->next;
            free(temp);
        } else {
            lb = lb->next;
        }
    }
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
    intersection_of_ordered_list(&l->next, lb->next);
    struct node_s* cur = l->next;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");

    return 0;
}
