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

static void odd_even_divide(struct node_s* a, struct node_s* b) {
    struct node_s* cur = a->next;
    struct node_s ahead = {0, NULL};
    struct node_s* pa = &ahead;
    struct node_s** pb = &b->next;
    struct node_s* temp;
    
    // idx = 0
    int i = 0;
    while (cur) {
        if (i % 2 == 0) {
            temp = cur->next;
            cur->next = *pb;
            *pb = cur;
            cur = temp;
        } else {
            pa->next = cur;
            pa = cur;
            cur = cur->next;
        }
        i++;
    }
    pa->next = NULL;
    a->next = ahead.next;
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

int main() {
    int arr[] = {1, 2, 3, 4, 5, 5, 6, 8, 5, 7, 4};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = list_create(arr, arr_len);

    struct node_s* lb = node_create(0);
    odd_even_divide(l, lb);

    struct node_s* cur = l->next;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
    cur = lb->next;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
    return 0;
}
