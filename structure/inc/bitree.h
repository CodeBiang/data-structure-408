#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include "basic.h"

typedef struct binode_s binode_t;
typedef struct binode_s bitree_t;

struct binode_s {
    binode_t* l;  // left_child
    binode_t* r;  // right_child

    basic_obj_t data;
};

typedef void (binode_traverse_fn)(binode_t* node, void* arg);

binode_t* binode_create();

void bitree_pre_traverse(bitree_t* node, binode_traverse_fn func, void* arg);
void bitree_in_traverse(bitree_t* node, binode_traverse_fn func, void* arg);
void bitree_post_traverse(bitree_t* node, binode_traverse_fn func, void* arg);
void bitree_floor_traverse(bitree_t* node, binode_traverse_fn func, void* arg);

#endif
