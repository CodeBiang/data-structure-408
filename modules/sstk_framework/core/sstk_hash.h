/**
 * @file    sstk_hash.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   通用hash
 */
#ifndef __SSTK_HASH_H_
#define __SSTK_HASH_H_

#include <sstk_core.h>

#define SSTK_RBNODE_TYPE_CODE 0
#define SSTK_LINKED_NODE_TYPE_CODE 1

typedef struct sstk_entry_node_s sstk_entry_node_t;
typedef sstk_entry_node_t sstk_map_linked_node_t;
typedef sstk_entry_node_t sstk_rbnode_t;

typedef int (*sstk_hash_code_fn)(void* ptr);
typedef int (*sstk_cmp_fn)(const void* ptr1, const void* ptr2);
typedef void* (*sstk_constructor_fn)(void* ptr, sstk_allocator_t* allocator);
typedef void (*sstk_destructor_fn)(void* ptr, sstk_allocator_t* allocator);

typedef struct {
    sstk_str_t name;
    sstk_constructor_fn constructor;
    sstk_destructor_fn destructor;
} sstk_allocable_t;

typedef struct {
    sstk_allocable_t* allocable;

    sstk_hash_code_fn hash_code;  // hashCode
    sstk_cmp_fn cmp;              // compare
} sstk_key_base_t;

/*!-- 用于获取size --*/
struct sstk_entry_node_s {
    unsigned char color : 1;
    int type : 3;   // entry 转换类型识别
    void* key;
    void* data;
    
    sstk_entry_node_t* next;
    sstk_entry_node_t* prev;

    sstk_rbnode_t* left;
    sstk_rbnode_t* right;
    sstk_rbnode_t* parent;
};

extern sstk_allocable_t sstk_default_allocable;
extern sstk_key_base_t sstk_default_key_base;

#endif
