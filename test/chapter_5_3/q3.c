/**
 * @file q3.c
 * @brief
 *
 * 后序遍历非递归
 *
 */
#include <stdio.h>

#include "bitree.h"
#include "linked_stack.h"

/**
 * @brief 非递归后序遍历
 *
 */
static void post_order_print(binode_t* node) {
    linked_stack_t stack;
    linked_stack_t* pstack = &stack;
    binode_t* out = NULL;
    binode_t* top = NULL;
    linked_node_t* temp;
    linked_stack_init(pstack);

    linked_stack_push(pstack)->data.obj = node;

    while (!linked_stack_empty(pstack)) {
        top = (binode_t*) linked_stack_top(pstack)->data.obj;

        if (out) {
            // 左子树出栈，右子树不为空
            if (out == top->l && top->r) {
                out = NULL;
                linked_stack_push(pstack)->data.obj = top->r;
            } else {
                printf("%s ", top->data.str);
                temp = linked_stack_pop(pstack);
                out = (binode_t*) temp->data.obj;
                free(temp);
            }
        } else if (top->l || top->r) {
            out = NULL;
            linked_stack_push(pstack)->data.obj = top->l ? top->l : top->r;
        } else {
            printf("%s ", top->data.str);
            temp = linked_stack_pop(pstack);
            out = (binode_t*) temp->data.obj;
            free(temp);
        }
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

    post_order_print(root);
    return 0;
}
