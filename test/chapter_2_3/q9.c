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

static void remove_by_range(struct node_s* list, int min, int max) {
    struct node_s* prev = list;
    struct node_s* cur = list->next;

    while (cur) {
        if (cur->data < max && cur->data > min) {
            prev->next = cur->next;
            free(cur);
            cur = prev->next;
        } else {
            prev = cur;
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

static void print_in_order(struct node_s* head) {
    struct node_s* cur;
    struct node_s* prev;
    struct node_s* min;
    struct node_s* min_prev;

    while (head->next) {
        cur = head->next;
        prev = head;
        min = head->next;
        min_prev = head;

        while (cur) {
            if (min->data > cur->data) {
                min = cur;
                min_prev = prev;
            }
            prev = cur;
            cur = cur->next;
        }

        min_prev->next = min->next;
        printf("%d ", min->data);
        free(min);
    }
    printf("\n");
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 5, 6, 8, 5, 7, 4};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = list_create(arr, arr_len);

    print_in_order(l);

    return 0;
}
