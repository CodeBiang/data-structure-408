/**
 * @file q4.c
 * @brief
 *
 * 右下角开始的层次遍历
 *
 */
#include <stdio.h>

#include "bitree.h"
#include "linked_queue.h"
#include "linked_stack.h"

/**
 * @brief 非递归后序遍历
 *
 */
static void reverse_floor_traverse(binode_t* node) {
    linked_stack_t stack;
    linked_stack_t* pstack = &stack;
    linked_queue_t queue;
    linked_queue_t* pqueue = &queue;
    binode_t* p;
    linked_node_t* temp;

    linked_stack_init(pstack);
    linked_queue_init(pqueue);

    linked_queue_enq(pqueue)->data.obj = node;

    while (!linked_queue_empty(pqueue)) {
        temp = linked_queue_deq(pqueue);
        p = (binode_t*) temp->data.obj;
        free(temp);
        linked_stack_push(pstack)->data.obj = p;
        if (p->l) linked_queue_enq(pqueue)->data.obj = p->l;
        if (p->r) linked_queue_enq(pqueue)->data.obj = p->r;
    }

    while (!linked_stack_empty(pstack)) {
        temp = linked_stack_pop(pstack);
        printf("%s ", ((binode_t*) temp->data.obj)->data.str);
        free(temp);
    }
    printf("\n");

    
    linked_stack_destory(pstack);
}

int main() {
    //       A
    //     B   C
    //      D E
    // 后序 D B E C A
    bitree_t* root = binode_create();
    root->l = binode_create();
    root->data.str = "A";
    root->l = binode_create();
    root->l->data.str = "B";
    root->l->r = binode_create();
    root->l->r->data.str = "D";
    root->r = binode_create();
    root->r->data.str = "C";
    root->r->l = binode_create();
    root->r->l->data.str = "E";

    reverse_floor_traverse(root);
    return 0;
}
