#include <stdio.h>
#include <string.h>

#include "tree.h"
// static void obj_print(basic_obj_t obj) { printf("%d ", obj.i); }

static void node_print(threaded_node_t* node, void* arg) { printf("%s ", node->data.str); }

#define threaded_unit_test(method)                      \
    do {                                                \
        threaded_tree_t* root = threaded_node_create(); \
        root->data.str = "A";                           \
        root->l = threaded_node_create();               \
        root->l->data.str = "B";                        \
        root->l->r = threaded_node_create();            \
        root->l->r->data.str = "D";                     \
        root->r = threaded_node_create();               \
        root->r->data.str = "C";                        \
        root->r->l = threaded_node_create();            \
        root->r->l->data.str = "E";                     \
        threaded_tree_##method##_thread(root);          \
        threaded_tree_##method##_traverse(root, node_print, NULL);       \
    } while (0)

static void test_pre() { threaded_unit_test(pre); }
static void test_in() { threaded_unit_test(in); }
static void test_post() { threaded_unit_test(post); }

int main() {
    //       A
    //     B   C
    //      D E
    // 先序 A B D C E
    // 中序 B D A E C
    // 后序 D B E C A
    test_pre();
    printf("\n");
    test_in();
    printf("\n");
    test_post();
    printf("\n");
    return 0;
}
