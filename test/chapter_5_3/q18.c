#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "linked_queue.h"
#include "threaded_tree.h"

struct param_s {
    int x;
    threaded_node_t* ret;
};

static threaded_node_t* get_post_prev(threaded_node_t* node) {
    if (!node->ltag) return node->l;

    if (!node->rtag) return node->r;

    if (!node->l) return NULL;

    while (node->ltag && node->l) node = node->l;

    return node->ltag == 0 ? node->l : NULL;
}

static void in_traverse(threaded_node_t* node, void* arg) {
    if (node->data.i == ((struct param_s*) arg)->x) {
        ((struct param_s*) arg)->ret = get_post_prev(node);
    }
}

threaded_node_t* in_order_post_prev(threaded_node_t* node, int x) {
    struct param_s param = {x, NULL};
    threaded_tree_in_traverse(node, in_traverse, &param);
    return param.ret;
}

int main() {
    //       1
    //   2       3
    //     4   5   6
    //              7
    // 后序 4 2 5 7 6 3 1
    threaded_node_t* root = threaded_node_create();
    root->l = threaded_node_create();
    root->data.i = 1;
    root->l = threaded_node_create();
    root->l->data.i = 2;
    root->l->r = threaded_node_create();
    root->l->r->data.i = 4;
    root->r = threaded_node_create();
    root->r->data.i = 3;
    root->r->l = threaded_node_create();
    root->r->l->data.i = 5;
    root->r->r = threaded_node_create();
    root->r->r->data.i = 6;
    root->r->r->r = threaded_node_create();
    root->r->r->r->data.i = 7;

    threaded_tree_in_thread(root);

    threaded_node_t* ret = in_order_post_prev(root, 7);
    printf("%d\n", ret->data.i);
    return 0;
}
