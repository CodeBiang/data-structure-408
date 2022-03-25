/**
 * @file    sstk_hash_map.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   数组+链表, str -> void*的哈希表
 */
#ifndef __SSTK_HASH_MAP_H_
#define __SSTK_HASH_MAP_H_

#include <sstk_core.h>

typedef struct {
    sstk_str_t name;
    sstk_uint_t hash;
} sstk_hash_key_t;

typedef struct {
    sstk_hash_key_t key;
    void* value;
} sstk_hash_entry_t;

typedef struct sstk_hash_node_s {
    sstk_hash_entry_t entry;
    struct sstk_hash_node_s* next;
} sstk_hash_node_t;

typedef struct {
    sstk_uint_t size;
    sstk_uint_t initial_capacity; /* 初始容量 */
    float load_factor;            /* 加载因子 */
    sstk_hash_node_t** buckets;   /* 哈希桶 */
    sstk_allocator_t* allocator;  /* 分配器 */
} sstk_hash_map_t;

typedef void (*sstk_hash_map_foreach_fn)(const sstk_hash_key_t* key, void* value);

sstk_int_t sstk_hash_map_init(sstk_hash_map_t* map, 
                              sstk_allocator_t* allocator,
                              sstk_uint_t initial_capacity, 
                              float load_factor);
sstk_int_t sstk_hash_map_default_init(sstk_hash_map_t* hash, 
                                      sstk_allocator_t* allocator);

sstk_int_t sstk_hash_map_put(sstk_hash_map_t* map, unsigned char* key, size_t key_len,
                             void* value);

void* sstk_hash_map_get(sstk_hash_map_t* map, unsigned char* key, size_t key_len);

void* sstk_hash_map_remove(sstk_hash_map_t* map, unsigned char* key, size_t key_len);

sstk_int_t sstk_hash_map_free(sstk_hash_map_t* map);

#define sstk_hash(hash, c) ((sstk_uint_t)hash * 31 + c)

static inline sstk_uint_t sstk_hash_code(unsigned char* data, size_t len) {
    sstk_uint_t i, key = 0;
    
    for (i = 0; i < len; i++) {
        key = sstk_hash(key, data[i]);
    }

    return key;
}

void sstk_hash_map_foreach(sstk_hash_map_t* map, sstk_hash_map_foreach_fn fn);


#endif
