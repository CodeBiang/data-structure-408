#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_queue.h"

static const char* bitree_pre_at_impl(bitree_t* node, int* i) {
    if (!node) return NULL;
    if (*i == 0) return node->data.str;
    const char* ret;
    (*i)--;
    
    ret = bitree_pre_at_impl(node->l, i);
    if (ret) return ret;
    return bitree_pre_at_impl(node->r, i);
}

const char* bitree_pre_at(bitree_t* node, int i) {
    return bitree_pre_at_impl(node, &i);
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
    const char* ret = bitree_pre_at(root, 3);
    printf("%s\n", ret ? ret : "null");
    return 0;
}
