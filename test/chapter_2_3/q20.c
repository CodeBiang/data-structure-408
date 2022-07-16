#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct node_s {
    int data;
    struct node_s* prev;
    struct node_s* next;
    int freq;
};

static inline struct node_s* node_create(int value) {
    struct node_s* ret = (struct node_s*) malloc(sizeof(struct node_s));
    ret->data = value;
    ret->prev = NULL;
    ret->next = NULL;
    ret->freq = 0;
    return ret;
}

static struct node_s* list_get(struct node_s* l, int x) {
    struct node_s* pre = l;
    struct node_s* cur = l->next;
    struct node_s* xcur;
    while (cur) {
        if (cur->data == x) {
            xcur = cur;
            pre->next = cur->next;
            cur->freq++;
            break;
        }
        pre = cur;
        cur = cur->next;
    }
    if (!cur) return NULL;

    pre = l;
    cur = l->next;
    while (cur) {
        if (xcur->freq > cur->freq) {
            pre->next = xcur;
            xcur->next = cur;
            break;
        }
        pre = cur;
        cur = cur->next;
    }

    if (!cur) {
        pre->next = xcur;
        xcur = NULL;
    }

    return xcur;
}

struct node_s* list_create(int arr[], size_t arr_len) {
    struct node_s* list = node_create(0);
    struct node_s* cur = list;

    for (size_t i = 0; i < arr_len; i++) {
        cur->next = node_create(arr[i]);
        cur->next->prev = cur;
        cur = cur->next;
    }

    return list;
}

static void print_list(struct node_s* l) {
    struct node_s* cur = l->next;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = list_create(arr, arr_len);
    list_get(l, 3);
    list_get(l, 3);
    list_get(l, 3);
    list_get(l, 2);
    list_get(l, 2);
    list_get(l, 1);
    list_get(l, 6);

    print_list(l);

    return 0;
}
