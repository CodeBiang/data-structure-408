#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_queue.h"

void bitree_free(binode_t* node) {
    if (!node) {
        return;
    }
    bitree_free(node->l);
    bitree_free(node->r);
    printf("free %d\n", node->data.i);
    free(node);
}

void bitree_pre_del(bitree_t* node, int i) {
    if (!node) return;
    if (node->data.i == i) {
        bitree_free(node);
    } else {
        bitree_pre_del(node->l, i);
        bitree_pre_del(node->r, i);
    }
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
    bitree_pre_del(root, 3);
    return 0;
}
