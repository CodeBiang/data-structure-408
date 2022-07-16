#include "threaded_tree.h"

#include <stdlib.h>

threaded_node_t* threaded_node_create() {
    threaded_node_t* node = (threaded_node_t*) malloc(sizeof(threaded_node_t));
    node->l = node->r = node->p = NULL;
    node->ltag = node->rtag = 0;
    node->data.obj = NULL;
    return node;
}

static void pre_order_thread(threaded_node_t* cur, threaded_node_t** pre) {
    if (cur == NULL) return;

    // 建立前驱链接
    if (cur->l == NULL) {
        cur->l = *pre;
        cur->ltag = 1;
    }

    // 前驱节点后节点 链接
    if ((*pre) != NULL && (*pre)->r == NULL) {
        (*pre)->r = cur;
        (*pre)->rtag = 1;
    }

    *pre = cur;
    if (!cur->ltag) pre_order_thread(cur->l, pre);
    if (!cur->rtag) pre_order_thread(cur->r, pre);
}

void threaded_tree_pre_thread(threaded_tree_t* root) {
    threaded_node_t* pre = NULL;
    pre_order_thread(root, &pre);
    if (pre) {
        pre->r = NULL;
        pre->rtag = 1;
    }
}

static void post_order_thread(threaded_node_t* cur, threaded_node_t* parent,
                              threaded_node_t** pre) {
    if (cur == NULL) return;

    if (parent) {
        cur->p = parent;
    }

    post_order_thread(cur->l, cur, pre);
    post_order_thread(cur->r, cur, pre);

    // 建立前驱链接
    if (cur->l == NULL) {
        cur->l = *pre;
        cur->ltag = 1;
    }

    // 前驱节点后节点 链接
    if ((*pre) != NULL && (*pre)->r == NULL) {
        (*pre)->r = cur;
        (*pre)->rtag = 1;
    }

    *pre = cur;
}

void threaded_tree_post_thread(threaded_tree_t* root) {
    threaded_node_t* pre = NULL;
    post_order_thread(root, NULL, &pre);
}

static void in_order_thread(threaded_node_t* cur, threaded_node_t** pre) {
    if (cur == NULL) return;

    in_order_thread(cur->l, pre);

    // 建立前驱链接
    if (cur->l == NULL) {
        cur->l = *pre;
        cur->ltag = 1;
    }

    // 前驱节点后节点 链接
    if ((*pre) != NULL && (*pre)->r == NULL) {
        (*pre)->r = cur;
        (*pre)->rtag = 1;
    }

    *pre = cur;

    in_order_thread(cur->r, pre);
}

void threaded_tree_in_thread(threaded_tree_t* root) {
    threaded_node_t* pre = NULL;
    in_order_thread(root, &pre);
    if (pre) {
        pre->r = NULL;
        pre->rtag = 1;
    }
}

static threaded_node_t* in_first_node(threaded_node_t* p) {
    while (p->ltag == 0) p = p->l;
    return p;
}

static threaded_node_t* in_next_node(threaded_node_t* p) {
    if (p->rtag == 0)
        return in_first_node(p->r);
    else
        return p->r;
}

void threaded_tree_in_traverse(threaded_tree_t* root, threaded_node_traverse_fn fn, void* arg) {
    for (threaded_node_t* p = in_first_node(root); p != NULL; p = in_next_node(p)) {
        fn(p, arg);
    }
}

static threaded_node_t* pre_first_node(threaded_node_t* p) { return p; }

static threaded_node_t* pre_next_node(threaded_node_t* p) {
    if (p->rtag) {
        // 后继线索已建立，返回已建立的后继节点
        return p->r;
    } else if (!p->ltag) {
        // 前驱线索未建立，则是非线索链接
        return p->l;
    } else {
        return p->r;
    }
}

void threaded_tree_pre_traverse(threaded_tree_t* root, threaded_node_traverse_fn fn, void* arg) {
    for (threaded_node_t* p = pre_first_node(root); p != NULL; p = pre_next_node(p)) {
        fn(p, arg);
    }
}

static threaded_node_t* post_first_node(threaded_node_t* node) {
    while ((!node->ltag && node->l) || (!node->rtag && node->r)) {
        node = node->l ? node->l : node->r;
    }
    return node;
}

static threaded_node_t* post_next_node(threaded_node_t* node) {
    if (node->rtag)
        return node->r;
    else {
        if (!node->p) return NULL;

        // 左子节点，且父节点右节点不是线索节点
        if (node == node->p->l) {
            if (!node->p->rtag) return post_first_node(node->p->r);
        } else {
            return node->p;
        }

        return NULL;
    }
}

void threaded_tree_post_traverse(threaded_tree_t* root, threaded_node_traverse_fn fn, void* arg) {
    for (threaded_node_t* node = post_first_node(root); node; node = post_next_node(node)) {
        fn(node, arg);
    }
}
