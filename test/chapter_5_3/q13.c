#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"

int ancestor_impl(binode_t* node, binode_t* p, binode_t* q, binode_t** r) {
    if (!node) return 0;
    if (p == node) return 1;
    if (q == node) return 2;

    int lret = ancestor_impl(node->l, p, q, r);
    int rret = ancestor_impl(node->r, p, q, r);
    if (lret && rret) {
        *r = node;
        return 3;
    } else if (lret)
        return lret;
    else if (rret)
        return rret;
    else
        return 0;
}

/**
 * @brief 祖先节点
 *
 */
bool ancestor(bitree_t* root, binode_t* p, binode_t* q, binode_t** r) {
    return ancestor_impl(root, p, q, r);
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
    binode_t* r = NULL;
    ancestor(root, root->r->l, root->r->r->r, &r);
    if (r) {
        printf("%d\n", r->data.i);
    }
    return 0;
}
