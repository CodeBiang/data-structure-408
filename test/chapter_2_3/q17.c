#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct node_s {
    int data;
    struct node_s* prev;
    struct node_s* next;
};

static inline struct node_s* node_dump(int value) {
    struct node_s* ret = (struct node_s*) malloc(sizeof(struct node_s));
    ret->data = value;
    ret->prev = NULL;
    ret->next = NULL;
    return ret;
}

static bool is_symmetric(struct node_s* l) {
    struct node_s* right = l->next;
    struct node_s* left = l->prev;

    while (left != right && left->next != right) {
        if (left->data != right->data) {
            return false;
        }
        left = left->prev;
        right = right->next;
    }

    return true;
}

struct node_s* circular_list_create(int arr[], size_t arr_len) {
    struct node_s* list = node_dump(0);
    struct node_s* cur = list;

    for (size_t i = 0; i < arr_len; i++) {
        cur->next = node_dump(arr[i]);
        cur->next->prev = cur;
        cur = cur->next;
    }

    cur->next = list;
    list->prev = cur;

    return list;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 6, 7, 6, 5, 4, 3, 2, 1};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = circular_list_create(arr, arr_len);
    bool ret = is_symmetric(l);

    printf("%s\n", ret ? "true" : "false");

    return 0;
}
