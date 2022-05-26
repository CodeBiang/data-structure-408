#include <stdbool.h>
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

static bool is_continuous_sublist(struct node_s* la, struct node_s* lb) {
    struct node_s* cur = lb;
    struct node_s* pre = NULL;
    struct node_s* acur = la;

    while (acur && cur) {
        if (acur->data == cur->data) {
            if (!pre) pre = acur;
            acur = acur->next;
            cur = cur->next;
        } else {
            if (pre) {
                acur = pre->next;
                pre = NULL;
                cur = lb;
            } else {
                acur = acur->next;
            }
        }
    }

    return cur == NULL;
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
    int arr[] = {1, 7, 6, 4, 5, 5, 7, 1, 7, 6, 5, 8};
    int arr1[] = {1, 7, 6, 5};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = list_create(arr, arr_len);
    struct node_s* lb = list_create(arr1, sizeof(arr1) / sizeof(int));
    bool ret = is_continuous_sublist(l->next, lb->next);

    printf("%s\n", ret ? "true" : "false");

    return 0;
}
