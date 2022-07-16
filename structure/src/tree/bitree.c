#include "bitree.h"

#include <stdlib.h>

#include "linked_queue.h"

binode_t* binode_create() {
    binode_t* node = (binode_t*) malloc(sizeof(binode_t));
    node->l = node->r = NULL;
    node->data.obj = NULL;
    return node;
}

void bitree_pre_traverse(bitree_t* node, binode_traverse_fn func, void* arg) {
    if (!node) return;
    func(node, arg);
    bitree_pre_traverse(node->l, func, arg);
    bitree_pre_traverse(node->r, func, arg);
}

void bitree_in_traverse(bitree_t* node, binode_traverse_fn func, void* arg) {
    if (!node) return;
    bitree_in_traverse(node->l, func, arg);
    func(node, arg);
    bitree_in_traverse(node->r, func, arg);
}

void bitree_post_traverse(bitree_t* node, binode_traverse_fn func, void* arg) {
    if (!node) return;
    bitree_post_traverse(node->l, func, arg);
    bitree_post_traverse(node->r, func, arg);
    func(node, arg);
}

void bitree_floor_traverse(bitree_t* node, binode_traverse_fn func, void* arg) {
    linked_queue_t q;
    linked_queue_init(&q);
    binode_t* p;
    linked_node_t* temp;

    linked_queue_enq(&q)->data.obj = node;
    while (!linked_queue_empty(&q)) {
        temp = linked_queue_deq(&q);
        p = (binode_t*) temp->data.obj;
        func(p, arg);
        free(temp);
        if (p->l) linked_queue_enq(&q)->data.obj = p->l;
        if (p->r) linked_queue_enq(&q)->data.obj = p->r;
    }
}
