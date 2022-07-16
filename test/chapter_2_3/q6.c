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

static struct node_s* merge_sort(struct node_s* l1, struct node_s* l2) {
    struct node_s head = {0, NULL};
    struct node_s* pcur = &head;

    while (l1 && l2) {
        if (l1->data < l2->data) {
            pcur->next = l1;
            l1 = l1->next;
        } else {
            pcur->next = l2;
            l2 = l2->next;
        }
        pcur = pcur->next;
    }

    pcur->next = l1 == NULL ? l2 : l1;

    return head.next;
}

static struct node_s* list_sort(struct node_s* list) {
    if (list == NULL || list->next == NULL) return list;

    struct node_s* lprev;
    struct node_s* llist = list;
    struct node_s* rlist = list;

    // 拆成两半
    while (rlist != NULL && rlist->next != NULL) {
        lprev = llist;
        llist = llist->next;
        rlist = rlist->next->next;
    }
    lprev->next = NULL;

    struct node_s* lhalf = list_sort(list);
    struct node_s* rhalf = list_sort(llist);

    return merge_sort(lhalf, rhalf);
}

int main() {
    int arr[] = {0, 5, 3, 4,  7, 2, 8, 6, 1};
    size_t arr_len = sizeof(arr) / sizeof(int);

    struct node_s* list = node_create(0);
    struct node_s* cur = list;

    for (size_t i = 0; i < arr_len; i++) {
        cur->next = node_create(arr[i]);
        cur = cur->next;
    }

    list_sort(list->next);

    cur = list->next;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
    return 0;
}
