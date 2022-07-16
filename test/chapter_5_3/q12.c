#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_queue.h"

/**
 * @brief 打印值为 x 的 所有 父节点，假设只有一个x
 * 
 * @param node 
 * @param i 
 */
bool parent_print(bitree_t* node, int x) {
    if (!node) return false;
    if (node->data.i == x) return true;
    if (parent_print(node->l, x)) {
        printf("%d\n", node->data.i);
        return true;
    }
    if (parent_print(node->r, x)) {
        printf("%d\n", node->data.i);
        return true;
    }
    return false;
}

int main() {
    //       1
    //   2       3
    //     4   5   6
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
    parent_print(root, 6);
    return 0;
}
