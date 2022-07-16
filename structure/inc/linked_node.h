#ifndef __LINKED_NODE__
#define __LINKED_NODE__

#include "basic.h"

#include <stdlib.h>

typedef struct linked_node_s linked_node_t;

struct linked_node_s {
    basic_obj_t data;
    linked_node_t* next;
};

#define linked_node_create() ((linked_node_t*) malloc(sizeof(linked_node_t)))

static inline linked_node_t* linked_node_link_create(linked_node_t* linked) {
    linked_node_t* node = linked_node_create();
    node->next = linked;
    return node;
}

typedef struct dlinked_node_s dlinked_node_t;

struct dlinked_node_s {
    basic_obj_t data;
    dlinked_node_t* prev;
    dlinked_node_t* next;
};

#define dlinked_node_create() ((dlinked_node_t*) malloc(sizeof(dlinked_node_t)))

#endif
