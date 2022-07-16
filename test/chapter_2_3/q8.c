#include <stdio.h>
#include <stdlib.h>

struct node_s {
    int data;
    struct node_s* next;
};

static inline struct node_s* node_create(int value) {
    struct node_s* ret = (struct node_s*) malloc(sizeof(struct node_s));
    ret->data = value;
    ret->next = NULL;
    return ret;
}

struct node_s* list_create(int arr[], size_t arr_len) {
    struct node_s* list = node_create(0);
    struct node_s* cur = list;

    for (size_t i = 0; i < arr_len; i++) {
        cur->next = node_create(arr[i]);
        cur = cur->next;
    }

    return list;
}

static void print_same(struct node_s* node1, struct node_s* node2) {
    struct node_s* cur;

    // BF
    while (node1) {
        cur = node2;
        while (cur) {
            if (cur->data == node1->data) {
                printf("%d ", cur->data);
                break;
            }
            cur = cur->next;
        }
        node1 = node1->next;
    }

    printf("\n");
}

int main() {
    int arr1[] = {1, 2, 3, 4, 5, 5, 6, 8, 5, 7, 4};
    size_t arr1_len = sizeof(arr1) / sizeof(int);
    int arr2[] = {1, 7, 11, 13, 15};
    size_t arr2_len = sizeof(arr2) / sizeof(int);

    struct node_s* l1 = list_create(arr1, arr1_len);
    struct node_s* l2 = list_create(arr2, arr2_len);

    print_same(l1->next, l2->next);

    return 0;
}
