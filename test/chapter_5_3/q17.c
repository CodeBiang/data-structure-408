#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_queue.h"

#define is_leaf_node(node) ((node)->l == (node)->r && !(node)->l)

bool similar_tree(binode_t* n1, binode_t* n2) {
    if (n1 == n2 && n1 == NULL)
        return true;
    else if (n1 == NULL || n2 == NULL)
        return false;

    if (is_leaf_node(n1) && is_leaf_node(n2)) return true;

    if (similar_tree(n1->l, n2->l) && similar_tree(n1->r, n2->r)) return true;
    
    return false;
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

    bitree_t* root1 = binode_create();
    root1->l = binode_create();
    root1->data.i = 1;
    root1->l = binode_create();
    root1->l->data.i = 2;
    root1->l->r = binode_create();
    root1->l->r->data.i = 4;
    root1->r = binode_create();
    root1->r->data.i = 3;
    root1->r->l = binode_create();
    root1->r->l->data.i = 5;
    root1->r->r = binode_create();
    root1->r->r->data.i = 6;
    root1->r->r->r = binode_create();
    root1->r->r->r->data.i = 7;

    printf("is similar: %d\n", similar_tree(root, root1));

    return 0;
}
