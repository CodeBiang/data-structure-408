#include <sstk_core.h>

#define SSTK_HASH_DEFAULT_INITIAL_CAPACITY 16
#define SSTK_HASH_DEFAULT_LOAD_FACTOR 0.75F

static sstk_int_t sstk_hash_map_rebuild(sstk_hash_map_t* map) {
    sstk_uint_t new_size = map->initial_capacity * 2;
    size_t buckets_size = sizeof(sstk_hash_node_t*) * new_size;
    sstk_hash_node_t** new_list;
    size_t n = map->size;
    sstk_hash_node_t *cur, *tmp;
    sstk_uint_t new_index;

    // assert new_size > size;

    // 重新分配
    if (map->allocator->alloc_fn) {
        new_list = map->allocator->alloc_fn(buckets_size);
    }

    if (!new_list) {
        return SSTK_ERR;
    }

    sstk_memzero(new_list, buckets_size);

    // 内存复制
    while (n--) {
        cur = map->buckets[n];

        while (cur) {
            // 缓存下个节点
            tmp = cur->next;

            // 新索引
            new_index = cur->entry.key.hash % new_size;

            // 表头添加
            cur->next = new_list[new_index];
            new_list[new_index] = cur; 

            // 下个节点遍历
            cur = tmp;
        }
    }

    // 释放旧的数组
    if (map->allocator->dealloc_fn) {
        map->allocator->dealloc_fn(map->buckets);
    } else {
        return SSTK_ERR;
    }

    map->buckets = new_list;
    map->initial_capacity = new_size;

    return SSTK_OK;
}

sstk_int_t sstk_hash_map_init(sstk_hash_map_t* map,
                              sstk_allocator_t* allocator,
                              sstk_uint_t initial_capacity, float load_factor) {
    size_t buckets_size = sizeof(sstk_hash_node_t*) * initial_capacity;

    map->size = 0;
    map->initial_capacity = initial_capacity;
    map->load_factor = load_factor;
    map->allocator = allocator ? allocator : sstk_cycle->allocator;

    if (map->allocator->alloc_fn)
        map->buckets = map->allocator->alloc_fn(buckets_size);

    if (map->buckets) {
        sstk_memzero(map->buckets, buckets_size);
        return SSTK_OK;
    } else {
        return SSTK_ERR;
    }
}

sstk_int_t sstk_hash_map_default_init(sstk_hash_map_t* map, 
                                      sstk_allocator_t* allocator) {
    return sstk_hash_map_init(map, allocator, SSTK_HASH_DEFAULT_INITIAL_CAPACITY,
                              SSTK_HASH_DEFAULT_LOAD_FACTOR);
}

sstk_int_t sstk_hash_map_put(sstk_hash_map_t* map, unsigned char* key, size_t key_len,
                             void* value) {
    sstk_hash_node_t* cur;
    sstk_uint_t hash_code = sstk_hash_code(key, key_len);
    sstk_uint_t index = hash_code % map->initial_capacity;

    if (!map->buckets) {
        sstk_log(sstk_cycle->logger, SSTK_LOG_ERROR, "null pointer");
        return SSTK_ERR;
    }

    cur = map->buckets[index];

    while (cur) {
        if (cur->entry.key.name.len == key_len
            && sstk_memcmp(key, cur->entry.key.name.data, key_len) == 0) {
            break;
        }
        cur = cur->next;
    }

    if (cur) {
        // key 存在
        cur->entry.value = value;
    } else {
        // key 不存在
        if (map->allocator->alloc_fn)
            cur = map->allocator->alloc_fn(sizeof(sstk_hash_node_t));
        if (cur) {
            cur->entry.key.hash = hash_code;
            cur->entry.key.name.data = key;
            cur->entry.key.name.len = key_len;
            cur->entry.value = value;

            // 表头添加
            cur->next = map->buckets[index];
            map->buckets[index] = cur;

            map->size++;

            // 扩容判断
            if (map->size > map->initial_capacity * map->load_factor) {
                if (sstk_hash_map_rebuild(map) != SSTK_OK) {
                    return SSTK_ERR;
                }
            }
        } else
            return SSTK_ERR;
    }

    return SSTK_OK;
}

void* sstk_hash_map_get(sstk_hash_map_t* map, unsigned char* key, size_t key_len) {
    sstk_uint_t hash_code = sstk_hash_code(key, key_len);
    hash_code %= map->initial_capacity;

    sstk_hash_node_t* cur = map->buckets[hash_code];

    while (cur) {
        if (cur->entry.key.name.len == key_len
            && sstk_memcmp(key, cur->entry.key.name.data, key_len) == 0) {
            break;
        }
        cur = cur->next;
    }

    return cur == NULL ? NULL : cur->entry.value;
}

void* sstk_hash_map_remove(sstk_hash_map_t* map, unsigned char* key, size_t key_len) {
    sstk_uint_t hash_code = sstk_hash_code(key, key_len);
    hash_code %= map->initial_capacity;

    sstk_hash_node_t* cur = map->buckets[hash_code];
    sstk_hash_node_t* prev = NULL;
    void* val = NULL;

    while (cur) {
        if (cur->entry.key.name.len == key_len
            && sstk_memcmp(key, cur->entry.key.name.data, key_len) == 0) {
            break;
        }
        prev = cur;
        cur = cur->next;
    }

    if (cur) {
        if (prev == NULL) {
            val = map->buckets[hash_code];
            map->buckets[hash_code] = NULL;
        } else {
            prev->next = cur->next;
            val = cur->entry.value;
        }

        map->allocator->dealloc_fn(cur);
    }

    return val;
}

sstk_int_t sstk_hash_map_free(sstk_hash_map_t* map) {
    sstk_hash_node_t* cur;
    sstk_hash_node_t* tmp;
    sstk_uint_t n = map->initial_capacity;

    if (!map || !map->buckets || !map->allocator->dealloc_fn) {
        sstk_log(sstk_cycle->logger, SSTK_LOG_ERROR, "null pointer");
        return SSTK_ERR;
    }

    while (n--) {
        cur = map->buckets[n];
        while (cur) {
            tmp = cur->next;
            map->allocator->dealloc_fn(cur);
            cur = tmp;
        }
    }

    map->allocator->dealloc_fn(map->buckets);

    map->buckets = NULL;

    return SSTK_OK;
}

void sstk_hash_map_foreach(sstk_hash_map_t* map, sstk_hash_map_foreach_fn fn) {
    sstk_uint_t i;
    sstk_hash_node_t* node;

    if (!fn) return;

    for (i = 0; i < map->initial_capacity; i++) {
        node = map->buckets[i];
        
        while (node) {
            fn(&node->entry.key, node->entry.value);
            
            node = node->next;
        }
    }
}