#include <stdbool.h>
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

static bool reciprocal_k_recursive(struct node_s* node, int* k) {
    // 终止条件
    if (node == NULL) {
        return false;
    }
    // 后序计算k
    if (!reciprocal_k_recursive(node->next, k)) {
        // 第k个
        if (*k == 0) {
            printf("%d\n", node->data);
            return true;
        } else if (*k < 0)
            return false;
        // k--
        *k = *k - 1;
        return false;
    } else
        return true;
}

static bool reciprocal_k(struct node_s* l, int k) { return reciprocal_k_recursive(l->next, &k); }

static bool reciprocal_k2(struct node_s* l, int k) {
    // p, q平移
    struct node_s *p, *q;

    p = l;
    q = l;
    
    while (q && k >= 0) {
        k--;
        q = q->next;
    }

    if (!q) return false;

    while (q) {
        p = p->next;
        q = q->next;
    }

    printf("%d\n", p->data);
    return true;
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
    int arr[] = {1, 2, 3, 4, 5, 6};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* l = list_create(arr, arr_len);

    printf("%d\n", reciprocal_k(l, 2));
    printf("%d\n", reciprocal_k2(l, 2));

    return 0;
}
