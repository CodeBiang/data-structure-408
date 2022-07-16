#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "linked_node.h"

typedef struct {
    int length;
    linked_node_t* head;
    linked_node_t* tail;
} linked_list_t;

bool linked_list_init(linked_list_t* l);

void linked_list_destory(linked_list_t* l);

linked_node_t* linked_list_insert_head(linked_list_t* l);

linked_node_t* linked_list_insert_tail(linked_list_t* l);

linked_node_t* linked_list_insert(linked_list_t* l, int idx);

bool linked_list_delete(linked_list_t* l, int idx, basic_obj_t* deleted_item);

/**
 * @brief 获取某个元素的下标
 * 
 * @param l 
 * @param elem 
 * @param equals 传入NULL 则进行memcmp
 * @return int 
 */
int linked_list_index_of(linked_list_t* l, basic_obj_t* elem, basic_obj_equals_fn equals);

linked_node_t* linked_list_at(linked_list_t* l, int idx);

void linked_list_foreach(linked_list_t* l, basic_obj_accept_fn accept);

#define linked_list_empty(l) ((l)->length == 0)

typedef struct {
    int length;
    dlinked_node_t* head;
    dlinked_node_t* tail;
} dlinked_list_t;

bool dlinked_list_init(dlinked_list_t* l);

void dlinked_list_destory(dlinked_list_t* l);

dlinked_node_t* dlinked_list_insert_head(dlinked_list_t* l);

dlinked_node_t* dlinked_list_insert_tail(dlinked_list_t* l);

dlinked_node_t* dlinked_list_insert(dlinked_list_t* l, int idx);

bool dlinked_list_delete(dlinked_list_t* l, int idx, basic_obj_t* deleted_item);

/**
 * @brief 获取某个元素的下标
 * 
 * @param l 
 * @param elem 
 * @param equals 传入NULL 则进行memcmp
 * @return int 
 */
int dlinked_list_index_of(dlinked_list_t* l, basic_obj_t* elem, basic_obj_equals_fn equals);

dlinked_node_t* dlinked_list_at(dlinked_list_t* l, int idx);

void dlinked_list_foreach(dlinked_list_t* l, basic_obj_accept_fn accept);

#define dlinked_list_empty(l) ((l)->length == 0)

#endif
