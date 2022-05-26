#include <limits.h>
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

static void ordered_print_and_del(struct node_s* l) {
    struct node_s* cur = l->next;
    struct node_s* pre = l;

    struct node_s* min_node = cur;
    struct node_s* min_pre = pre;
    while (l->next != l) {
        if (cur == l) {
            printf("%d ", min_node->data);

            min_pre->next = min_node->next;
            free(min_node);
            cur = l->next;
            pre = l;
            min_node = cur;
            min_pre = pre;
        } else {
            if (cur->data < min_node->data) {
                min_node = cur;
                min_pre = pre;
            }
        }
        pre = cur;
        cur = cur->next;
    }

    free(l);
    printf("\n");
}

struct node_s* circular_list_create(int arr[], size_t arr_len) {
    struct node_s* list = node_dump(0);
    struct node_s* cur = list;

    for (size_t i = 0; i < arr_len; i++) {
        cur->next = node_dump(arr[i]);
        cur = cur->next;
    }

    cur->next = list;

    return list;
}

int main() {
    int arr[] = {1, 7, 6, 8, 4, 3, 2, 15};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = circular_list_create(arr, arr_len);
    ordered_print_and_del(l);
    return 0;
}
