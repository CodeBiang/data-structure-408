#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_queue.h"

int double_children_count(bitree_t* node) {
    if (!node) return 0;
    int cnt = 0;
    if (node->l && node->r) cnt++;
    cnt += double_children_count(node->l);
    cnt += double_children_count(node->r);
    return cnt;
}

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

    printf("%d\n", double_children_count(root));
    return 0;
}
