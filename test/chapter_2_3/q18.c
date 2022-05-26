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

static void circular_list_combine(struct node_s* la, struct node_s* lb) {
    struct node_s* btail = lb;
    struct node_s* temp = la->next;
    
    
    while (btail->next != lb) {
        btail = btail->next;
    }

    la->next = lb;
    btail->next = temp;
}

struct node_s* circular_list_create(int arr[], size_t arr_len) {
    struct node_s* cur;
    struct node_s* ret = NULL;
    struct node_s* pre = NULL;

    for (size_t i = 0; i < arr_len; i++) {
        cur = node_dump(arr[i]);
        if (!ret) ret = cur;
        
        if (pre) pre->next = cur;
        pre = cur;
    }

    cur->next = ret;

    return ret;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6};
    size_t arr_len = sizeof(arr) / sizeof(int);
    int arr1[] = {7, 6, 5, 4, 3, 2, 1};
    size_t arr1_len = sizeof(arr1) / sizeof(int);

    struct node_s* la = circular_list_create(arr, arr_len);
    struct node_s* lb = circular_list_create(arr1, arr1_len);
    circular_list_combine(la, lb);

    struct node_s* cur = la;
    while (cur->next != la) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("%d ", cur->data);
    printf("\n");
    return 0;
}
