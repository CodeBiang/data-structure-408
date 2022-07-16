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

// O(n)标准答案
static bool is_circular2(struct node_s* l) {
    struct node_s* fast = l->next;
    struct node_s* slow = l->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) break;
    }

    if (slow == NULL || !fast || !fast->next)
        return false;
    else
        return true;
}

// O(logn) 暴力解算
static bool is_circular(struct node_s* l) {
    if (l->next == NULL || l->next->next == NULL) return false;

    struct node_s* pre = l->next;
    struct node_s* cur = l->next->next;
    struct node_s* node = l->next;

    while (cur) {
        node = l->next;
        while (node != pre && node != cur) {
            node = node->next;
        }

        if (node == cur) return true;

        pre = cur;
        cur = cur->next;
    }

    return false;
}

static struct node_s* list_get(struct node_s* l, int x) {
    struct node_s* cur = l->next;
    while (cur) {
        if (cur->data == x) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
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

    list_get(l, 6)->next = list_get(l, 3);

    printf("%d\n", is_circular(l));
    printf("%d\n", is_circular2(l));

    return 0;
}
