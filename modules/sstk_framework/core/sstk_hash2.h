/**
 * @file    sstk_map.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   哈希表不定类型的适配版本
 *
 * @todo    数组+链表+红黑树, void* -> void*的哈希表
 *
 * @c       2021/11/23 完成移植原型设计
 *
 * @todo    暂未实现
 */
#ifndef __SSTK_HASH2_H_
#define __SSTK_HASH2_H_

#include <sstk_core.h>

#define SSTK_MAP_LINKED_LIST_BUCKET 0
#define SSTK_MAP_RBTREE_BUCKET 1

/*!-- 8 个泛型适配接口 --*/
typedef struct {
    void* reserved[8];
} sstk_map_if_t;

typedef struct {
    int type;
    union {
        sstk_map_linked_node_t* linked;
        sstk_rbtree_t* tree;
    } data;
} sstk_map_bucket_t;

typedef struct {
    sstk_map_if_t map_if;

    sstk_key_base_t* key_base;   // key 类型构造声明
    sstk_allocable_t* val_base;  // val 类型构造声明

    // treeify 过程中, key 和 val 都是无需再次构造的
    // allocable的constructor为空构造
    sstk_key_base_t tree_key_base;
    sstk_allocable_t tree_val_base;

    sstk_uint_t size;
    sstk_uint_t initial_capacity;  // 初始容量
    float load_factor;             // 加载因子
    sstk_map_bucket_t* buckets;    // 哈希桶
    sstk_allocator_t* allocator;   // 分配器
} sstk_map_t;

sstk_int_t sstk_map_init(sstk_map_t* map, sstk_allocator_t* allocator, sstk_uint_t initial_capacity,
                         float load_factor, sstk_key_base_t* key_base, sstk_allocable_t* val_base);

sstk_int_t sstk_map_default_init(sstk_map_t* map, sstk_allocator_t* allocator,
                                 sstk_key_base_t* key_base, sstk_allocable_t* val_base);
sstk_int_t sstk_map_put(sstk_map_t* map, void* key, void* value);

void* sstk_map_get(sstk_map_t* map, unsigned char* key, size_t key_len);

void* sstk_map_remove(sstk_map_t* map, unsigned char* key, size_t key_len);

sstk_int_t sstk_map_free(sstk_map_t* map);

#endif  // __SSTK_HASH2_H_
