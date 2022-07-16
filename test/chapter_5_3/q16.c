#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_queue.h"

/**
 * @brief 叶子节点从左到右链接
 *
 * 1. 从左到右的叶节点 ==> 右节点先访问，因为链表头插法结果是逆序的
 * 2. 叶子节点 ==> l r 为NULL
 *
 */
void leaf_link(binode_t* node, binode_t* head) {
    if (!node) return;

    if (!node->l && !node->r) {
        node->r = head->r;
        head->r = node;
    } else {
        leaf_link(node->r, head);
        leaf_link(node->l, head);
    }
}

int main() {
    //       1
    //   2       3
    //     4   5   6
    //              7
    bitree_t* root = binode_create();
    root->l = binode_create();
    root->data.i = 1;
    root->l = binode_create();
    root->l->data.i = 2;
    root->l->r = binode_create();
    root->l->r->data.i = 4;
    root->r = binode_create();
    root->r->data.i = 3;
    root->r->l = binode_create();
    root->r->l->data.i = 5;
    root->r->r = binode_create();
    root->r->r->data.i = 6;
    root->r->r->r = binode_create();
    root->r->r->r->data.i = 7;

    binode_t head = {NULL};
    binode_t* cur;
    leaf_link(root, &head);

    cur = head.r;
    while (cur) {
        printf("%d ", cur->data.i);
        cur = cur->r;
    }
    printf("\n");

    return 0;
}
