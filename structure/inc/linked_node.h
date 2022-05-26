#ifndef __LINKED_NODE__
#define __LINKED_NODE__

#include "basic.h"

typedef struct linked_node_s linked_node_t;

struct linked_node_s {
    basic_obj_t data;
    linked_node_t* next;
};

typedef struct dlinked_node_s dlinked_node_t;

struct dlinked_node_s {
    basic_obj_t data;
    dlinked_node_t* prev;
    dlinked_node_t* next;
};

#endif
