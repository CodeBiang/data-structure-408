/**
 * @file    sstk_rbtree.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   红黑树基础数据类型封装
 * 
 */
#ifndef __SSTK_RBTREE_IF_H_
#define __SSTK_RBTREE_IF_H_

#include <sstk_core.h>

typedef struct sstk_int_rbtree_if_s sstk_rbtree_int_if_t;
typedef struct sstk_str_rbtree_if_s sstk_rbtree_str_if_t;

struct sstk_int_rbtree_if_s {
    int (*put)(sstk_rbtree_int_if_t* tree, int key, void* val);
    void (*remove)(sstk_rbtree_int_if_t* tree, int key);
    const sstk_rbnode_t* (*get)(sstk_rbtree_int_if_t* tree, int key, void** pval);

    void* reserved[5];
};

struct sstk_str_rbtree_if_s {
    int (*put)(sstk_rbtree_str_if_t* tree, sstk_str_t* key, void* val);
    void (*remove)(sstk_rbtree_str_if_t* tree, sstk_str_t* key);
    const sstk_rbnode_t* (*get)(sstk_rbtree_str_if_t* tree, sstk_str_t* key, void** pval);

    void* reserved[5];
};

int sstk_int_rbtree_init(sstk_rbtree_t* tree, sstk_allocable_t* val_allocatable, sstk_allocator_t* allocator);

int sstk_str_rbtree_init(sstk_rbtree_t* tree, sstk_allocable_t* val_allocatable, sstk_allocator_t* allocator);

#endif
