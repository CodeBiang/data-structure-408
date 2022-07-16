#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_stack.h"

int get_height(bitree_t* node) {
    if (!node) return 0;

    int h = 1, max = 0;
    binode_t* p = node;
    linked_stack_t* st = (linked_stack_t*) malloc(sizeof(linked_stack_t));
    linked_stack_init(st);

    while (p || !linked_stack_empty(st)) {
        if (p) {
            h++;
            if (h > max) max = h;
            linked_stack_push(st)->data.obj = p;
            p = p->l;
        } else {
            h--;
            p = (binode_t*) linked_stack_pop(st)->data.obj;
            p = p->r;
        }
    }

    linked_stack_destory(st);
    
    return max;
}

int main() {
    //       A
    //   B       C
    //     D   E
    //    F
    // 后序 D B E C A
    bitree_t* root = binode_create();
    root->l = binode_create();
    root->data.str = "A";
    root->l = binode_create();
    root->l->data.str = "B";
    root->l->r = binode_create();
    root->l->r->data.str = "D";
    root->l->r->l = binode_create();
    root->l->r->l->data.str = "F";
    root->r = binode_create();
    root->r->data.str = "C";
    root->r->l = binode_create();
    root->r->l->data.str = "E";
    get_height(root);
    return 0;
}
