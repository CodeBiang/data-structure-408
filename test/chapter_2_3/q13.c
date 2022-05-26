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

static struct node_s* reverse_ab(struct node_s* la, struct node_s* lb) {
    struct node_s* lc = NULL;
    struct node_s* temp;
    
    while (la && lb) {
        if (la->data < lb->data) {
            temp = la;
            la = la->next;
            temp->next = lc;
            lc = temp;
        } else {
            temp = lb;
            lb = lb->next;
            temp->next = lc;
            lc = temp;
        }
    }

    while (la) {
        temp = la;
        la = la->next;
        temp->next = lc;
        lc = temp;
    }
    while (lb) {
        temp = lb;
        lb = lb->next;
        temp->next = lc;
        lc = temp;
    }

    return lc;
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
    int arr[] = {1, 2, 4, 5, 7, 8};
    int arr1[] = {0, 3, 6};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = list_create(arr, arr_len);
    struct node_s* lb = list_create(arr1, sizeof(arr1) / sizeof(int));

    struct node_s* cur = reverse_ab(l->next, lb->next);
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");

    return 0;
}
