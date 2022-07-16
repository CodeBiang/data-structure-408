#ifndef __THREADED_TREE_H__
#define __THREADED_TREE_H__

#include "basic.h"

typedef struct threaded_node_s threaded_node_t;
typedef struct threaded_node_s threaded_tree_t;

struct threaded_node_s {
    threaded_node_t* p;  // parent
    threaded_node_t* l;  // left_child
    threaded_node_t* r;  // right_child
    int ltag, rtag;

    basic_obj_t data;
};

typedef void (threaded_node_traverse_fn)(threaded_node_t* node, void* arg);

threaded_node_t* threaded_node_create();

void threaded_tree_pre_thread(threaded_tree_t* root);
void threaded_tree_post_thread(threaded_tree_t* root);
void threaded_tree_in_thread(threaded_tree_t* root);

void threaded_tree_pre_traverse(threaded_tree_t* root, threaded_node_traverse_fn fn, void* arg);
void threaded_tree_post_traverse(threaded_tree_t* root, threaded_node_traverse_fn fn, void* arg);
void threaded_tree_in_traverse(threaded_tree_t* root, threaded_node_traverse_fn fn, void* arg);

#endif
