#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void solve_list(struct node_s* l) {
    struct node_s l1 = {0, NULL};
    struct node_s l2 = {0, NULL};
    struct node_s *p, *temp, **q;

    p = &l1;
    q = &l2.next;
    int n, i;
    int half;
    struct node_s* cur = l->next;

    for (n = 0; cur; n++) cur = cur->next;
    half = n / 2;

    for (i = 0, cur = l->next; cur; i++) {
        if (i < half) {
            p->next = cur;
            p = cur;
            cur = cur->next;
        } else {
            temp = cur->next;
            cur->next = *q;
            *q = cur;
            cur = temp;
        }
    }
    p->next = NULL;

    // 合并
    cur = l;
    p = l1.next;
    q = &l2.next;
    for (i = 0; p && *q; i++) {
        if (i % 2 == 0) {
            cur->next = p;
            p = p->next;
        } else {
            cur->next = *q;
            *q = (*q)->next;
        }
        cur = cur->next;
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = list_create(arr, arr_len);
    solve_list(l);
    struct node_s* cur = l->next;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
    return 0;
}
