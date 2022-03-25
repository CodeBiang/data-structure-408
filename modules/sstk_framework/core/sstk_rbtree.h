/**
 * @file    sstk_rbtree.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   红黑树
 *
 * @c       2021/11/23 完成增删改查功能
 * 
 * 工具: https://rbtree.phpisfuture.com/
 * 特点:
 * 1. 节点要么是红色要么是黑色
 * 2. 根节点为黑色
 * 3. 叶子节点即NIL节点必定为黑色
 * 4. 红色节点的孩子节点必定为黑色
 * 5. 从任一节点到叶子节点，所包含的黑色节点数目相同，即黑高度相同
 */
#ifndef __SSTK_RBTREE_H_
#define __SSTK_RBTREE_H_

#include <sstk_core.h>

typedef struct sstk_rbtree_s sstk_rbtree_t;
typedef struct sstk_rbtree_if_s sstk_rbtree_if_t;

struct sstk_rbtree_if_s {
    int (*put)(sstk_rbtree_if_t* tree, void* key, void* val);
    void (*remove)(sstk_rbtree_if_t* tree, void* key);
    const sstk_rbnode_t* (*get)(sstk_rbtree_if_t* tree, void* key, void** pval);

    void* reserved[5];
};

struct sstk_rbtree_s {
    sstk_rbtree_if_t tree_if;           // 接口
    sstk_key_base_t* key_base;
    sstk_allocable_t* val_base;
    sstk_allocator_t* allocator;

    sstk_rbnode_t* root;                // 根节点
    sstk_rbnode_t* nil;                 // nil

    size_t size;
};

int sstk_rbtree_init(sstk_rbtree_t* tree, sstk_key_base_t* base, sstk_allocable_t* val_base,
                     sstk_allocator_t* allocator);
int sstk_rbtree_put(sstk_rbtree_t* tree, void* key, void* value);
int sstk_rbtree_put_node(sstk_rbtree_t* tree, sstk_rbnode_t* node);
void sstk_rbtree_remove(sstk_rbtree_t* tree, void* key);
void sstk_rbtree_remove_node(sstk_rbtree_t* tree, sstk_rbnode_t* node);
const sstk_rbnode_t* sstk_rbtree_get(sstk_rbtree_t* tree, void* key, void** data);
void sstk_rbtree_clear(sstk_rbtree_t* tree);
void sstk_rbtree_free(sstk_rbtree_t* tree);


sstk_rbnode_t* sstk_rbtree_next(sstk_rbtree_t* tree, sstk_rbnode_t* node);

static inline sstk_rbnode_t* sstk_rbtree_min(sstk_rbnode_t* node, sstk_rbnode_t* nil) {
    while (node->left != nil) {
        node = node->left;
    }

    return node;
}

#define sstk_rbtree_foreach(node, tree) \
    for (node = sstk_rbtree_min((tree)->root, (tree)->nil); node; node = sstk_rbtree_next((tree), node))

#endif
