#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_queue.h"

/**
 * @brief 层次遍历 + 完全二叉树定义判断
 * 
 * @param node 
 * @return true 
 * @return false 
 */
bool is_entire_bitree(bitree_t* node) {
    linked_queue_t q;
    linked_queue_init(&q);
    binode_t* p;
    linked_node_t* temp;
    bool fail = false;

    linked_queue_enq(&q)->data.obj = node;
    while (!linked_queue_empty(&q)) {
        temp = linked_queue_deq(&q);
        p = (binode_t*) temp->data.obj;
        free(temp);

        if (fail) {
            if (!p->l && !p->r) continue;
            else break;
        }

        if (p->l) {
            linked_queue_enq(&q)->data.obj = p->l;
        } else
            fail = true;

        if (p->r) {
            if (fail) break;
            linked_queue_enq(&q)->data.obj = p->r;
        } else
            fail = true;
    }

    if (!linked_queue_empty(&q)) {
        linked_queue_destory(&q);
        return false;
    } else
        return true;
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
    // root->l->l = binode_create();
    // root->l->l->data.str = "X";
    root->l->r = binode_create();
    root->l->r->data.str = "D";
    root->r = binode_create();
    root->r->data.str = "C";
    root->r->l = binode_create();
    root->r->l->data.str = "E";
    root->r->r = binode_create();
    root->r->r->data.str = "Y";

    printf("%d\n", is_entire_bitree(root));
    return 0;
}
