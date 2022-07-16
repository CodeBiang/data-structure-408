#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_queue.h"

void lr_exchange(bitree_t* node) {
    if (!node) return;

    lr_exchange(node->l);
    lr_exchange(node->r);

    binode_t* temp = node->l;
    node->l = node->r;
    node->r = temp;
}

static void node_print(binode_t* node, void* arg) { printf("%s ", node->data.str); }

int main() {
    //       A
    //   B       C
    //     D   E
    //       A
    //   B       C
    //  X D     E Y
    bitree_t* root = binode_create();
    root->l = binode_create();
    root->data.str = "A";
    root->l = binode_create();
    root->l->data.str = "B";
    root->l->l = binode_create();
    root->l->l->data.str = "X";
    root->l->r = binode_create();
    root->l->r->data.str = "D";
    root->r = binode_create();
    root->r->data.str = "C";
    root->r->l = binode_create();
    root->r->l->data.str = "E";
    root->r->r = binode_create();
    root->r->r->data.str = "Y";
    lr_exchange(root);
    bitree_pre_traverse(root, node_print, NULL);
    printf("\n");
    return 0;
}
