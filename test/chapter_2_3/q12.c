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

static void ordered_list_del_same(struct node_s* l) {
    if (!l || !l->next) return;

    int cur_val = l->next->data;
    struct node_s* pre = l->next;
    struct node_s* cur = l->next->next;

    while (cur) {
        if (cur->data == cur_val) {
            pre->next = cur->next;
            free(cur);
            cur = pre->next;
        } else {
            cur_val = cur->data;
            pre = cur;
            cur = cur->next;
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
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = list_create(arr, arr_len);

    ordered_list_del_same(l);

    struct node_s* cur = l->next;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");

    return 0;
}
