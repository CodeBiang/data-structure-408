#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define _abs(x) ((x > 0) ? (x) : (-x))

static void remove_abs_same(struct node_s* l, int n) {
    // p, q平移
    struct node_s* pre = l;
    struct node_s* cur = l->next;

    unsigned char* bitmap = (unsigned char*) malloc((size_t) n);
    memset(bitmap, 0, (size_t) n);

    while (cur) {
        if (bitmap[_abs(cur->data)]) {
            pre->next = cur->next;
            free(cur);
            cur = pre->next;
        } else {
            bitmap[_abs(cur->data)] = 1;
            pre = cur;
            cur = cur->next;
        }
    }

    free(bitmap);
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
    int n = 10;
    int arr[] = {-11, 4, 11, 4, 11, 6};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = list_create(arr, arr_len);

    remove_abs_same(l, n);

    struct node_s* cur = l->next;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");

    return 0;
}
