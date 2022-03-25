#include <sstk_core.h>

#define SSTK_MAP_DEFAULT_INITIAL_CAPACITY 16
#define SSTK_MAP_DEFAULT_LOAD_FACTOR 0.75F
#define SSTK_MAP_LINKED_BUCKET_DEPTH 8

static void sstk_map_treeify(sstk_map_t* map, sstk_map_bucket_t* bucket);
static void* tree_empty_allocable_constructor(void* ptr, sstk_allocator_t* allocator) {
    return ptr;
}


static sstk_int_t sstk_map_rebuild(sstk_map_t* map) {
    sstk_uint_t new_size = map->initial_capacity * 2;
    size_t buckets_size = sizeof(sstk_map_bucket_t) * new_size;
    sstk_map_bucket_t* new_buckets;
    int* new_buckets_len;
    size_t n = map->size;
    sstk_map_bucket_t *bucket, *tar_bucket;
    sstk_map_linked_node_t *cur, *tmp;
    sstk_rbnode_t *tcur, *ttmp;

    sstk_uint_t new_index;

    // assert new_size > size;

    // 重新分配
    new_buckets = map->allocator->alloc_fn(buckets_size);
    new_buckets_len = map->allocator->alloc_fn(new_size * sizeof(int));

    sstk_memzero(new_buckets, buckets_size);
    sstk_memzero(new_buckets_len, new_size * sizeof(int));

    // 链表化
    while (n--) {
        bucket = &map->buckets[n];

        if (bucket->type == SSTK_MAP_LINKED_LIST_BUCKET) {
            // 链表节点遍历
            cur = bucket->data.linked;
            while (cur) {
                // 缓存下个节点
                tmp = cur->next;

                // 新索引
                new_index = map->key_base->hash_code(cur->key) % new_size;

                // 获取目标哈希桶
                tar_bucket = &new_buckets[new_index];

                // 插入
                if (tar_bucket->type == SSTK_MAP_LINKED_LIST_BUCKET) {
                    // 链表桶, 头插
                    cur->type = SSTK_LINKED_NODE_TYPE_CODE;
                    cur->next = tar_bucket->data.linked;
                    tar_bucket->data.linked = cur;
                    new_buckets_len[new_index]++;

                    if (new_buckets_len[new_index] >= SSTK_MAP_LINKED_BUCKET_DEPTH) {
                        // 转红黑树
                        // 链表转红黑树
                        sstk_map_treeify(map, tar_bucket);
                    }
                } else {
                    // 树桶, 强转为树节点
                    tcur = (sstk_rbnode_t*) cur;
                    tcur->type = SSTK_RBNODE_TYPE_CODE;

                    // 树插入
                    sstk_rbtree_put_node(tar_bucket->data.tree, tcur);
                }

                // 下个节点遍历
                cur = tmp;
            }

        } else {
            // treenode 可以直接转为 linkednode 进行使用
            // 树节点遍历
            tcur = sstk_rbtree_min((bucket->data.tree)->root, (bucket->data.tree)->nil);
            while (tcur) {
                ttmp = sstk_rbtree_next(bucket->data.tree, tcur);
                {
                    new_index = map->key_base->hash_code(tcur->key) % new_size;

                    // 获取目标哈希桶
                    tar_bucket = &new_buckets[new_index];

                    // 插入
                    if (tar_bucket->type == SSTK_MAP_LINKED_LIST_BUCKET) {
                        cur = (sstk_map_linked_node_t*) tcur;
                        cur->type = SSTK_LINKED_NODE_TYPE_CODE;
                        // 链表桶, 头插
                        cur->next = tar_bucket->data.linked;
                        tar_bucket->data.linked = cur;
                        new_buckets_len[new_index]++;

                        if (new_buckets_len[new_index] >= SSTK_MAP_LINKED_BUCKET_DEPTH) {
                            // 转红黑树
                            // 链表转红黑树
                            sstk_map_treeify(map, tar_bucket);
                        }
                    } else {
                        // 树桶, 强转为树节点
                        tcur->type = SSTK_RBNODE_TYPE_CODE;

                        // 树插入
                        sstk_rbtree_put_node(tar_bucket->data.tree, tcur);
                    }
                }
                tcur = ttmp;
            }
        }
    }

    // 释放旧的桶
    map->allocator->dealloc_fn(map->buckets);

    map->buckets = new_buckets;
    map->initial_capacity = new_size;

    // 释放临时长度记录
    map->allocator->dealloc_fn(new_buckets_len);

    return SSTK_OK;
}

sstk_int_t sstk_map_init(sstk_map_t* map, sstk_allocator_t* allocator, sstk_uint_t initial_capacity,
                         float load_factor, sstk_key_base_t* key_base, sstk_allocable_t* val_base) {
    size_t buckets_size = sizeof(sstk_map_bucket_t) * initial_capacity;

    map->size = 0;
    map->initial_capacity = initial_capacity;
    map->load_factor = load_factor;
    map->allocator = allocator ? allocator : sstk_cycle->allocator;
    map->key_base = key_base ? key_base : &sstk_default_key_base;
    map->val_base = val_base ? val_base : &sstk_default_allocable;

    sstk_memcpy(&map->tree_key_base, map->key_base, sizeof(sstk_key_base_t));
    sstk_memcpy(&map->tree_val_base, map->val_base, sizeof(sstk_allocable_t));

    map->tree_key_base.allocable->constructor = tree_empty_allocable_constructor;
    map->tree_val_base.constructor = tree_empty_allocable_constructor;

    if (map->allocator->alloc_fn) map->buckets = map->allocator->alloc_fn(buckets_size);

    if (map->buckets) {
        sstk_memzero(map->buckets, buckets_size);
        return SSTK_OK;
    } else {
        return SSTK_ERR;
    }
}

sstk_int_t sstk_map_default_init(sstk_map_t* map, sstk_allocator_t* allocator,
                                 sstk_key_base_t* key_base, sstk_allocable_t* val_base) {
    return sstk_map_init(map, allocator, SSTK_MAP_DEFAULT_INITIAL_CAPACITY,
                         SSTK_MAP_DEFAULT_LOAD_FACTOR, key_base, val_base);
}

static void sstk_map_treeify(sstk_map_t* map, sstk_map_bucket_t* bucket) {
    sstk_map_linked_node_t *cur, *temp;
    sstk_rbtree_t* tree;

    cur = bucket->data.linked;

    tree = map->allocator->alloc_fn(sizeof(sstk_rbtree_t));

    // tree 的 key 使用
    sstk_rbtree_init(tree, &map->tree_key_base, &map->tree_val_base, map->allocator);

    while (cur) {
        // 放入红黑树
        sstk_rbtree_put(tree, cur->key, cur->data);

        temp = cur;
        cur = cur->next;

        // 释放链表节点
        map->allocator->dealloc_fn(temp);
    }

    bucket->type = SSTK_MAP_RBTREE_BUCKET;
    bucket->data.tree = tree;
}

static sstk_int_t sstk_map_linked_put(sstk_map_t* map, sstk_map_bucket_t* bucket, void* key,
                                      void* val) {
    sstk_map_linked_node_t** linked_head = &(bucket->data.linked);
    sstk_map_linked_node_t* cur = *linked_head;
    int bucket_depth = 0;

    while (cur) {
        if (map->key_base->cmp(cur->key, key) == 0) {
            break;
        }
        cur = cur->next;
        bucket_depth++;
    }

    if (cur) {
        // key 存在
        // 释放 输入的key
        map->key_base->allocable->destructor(key, map->allocator);
        // 释放 existed val
        map->val_base->destructor(cur->data, map->allocator);
        // 替换新val
        cur->data = map->val_base->constructor(val, map->allocator);
    } else {
        // key 不存在
        cur = map->allocator->alloc_fn(sizeof(sstk_entry_node_t));

        cur->type = SSTK_LINKED_NODE_TYPE_CODE;
        cur->key = map->key_base->allocable->constructor(key, map->allocator);
        cur->data = map->val_base->constructor(val, map->allocator);

        // 表头添加
        cur->next = *linked_head;
        *linked_head = cur;

        if (bucket_depth >= SSTK_MAP_LINKED_BUCKET_DEPTH) {
            // 链表转红黑树
            sstk_map_treeify(map, bucket);
        }

        map->size++;

        // 扩容判断
        if (map->size > map->initial_capacity * map->load_factor) {
            if (sstk_map_rebuild(map) != SSTK_OK) {
                return SSTK_ERR;
            }
        }
    }

    return SSTK_OK;
}

sstk_int_t sstk_map_put(sstk_map_t* map, void* key, void* value) {
    sstk_map_bucket_t* cur;
    sstk_uint_t hash_code = map->key_base->hash_code(key);
    sstk_uint_t index = hash_code % map->initial_capacity;
    void* constructed_key;
    void* constructed_val;

    if (!map->buckets) {
        sstk_log(sstk_cycle->logger, SSTK_LOG_ERROR, "null pointer");
        return SSTK_ERR;
    }

    cur = &map->buckets[index];

    if (cur->type == SSTK_MAP_LINKED_LIST_BUCKET) {
        return sstk_map_linked_put(map, cur, key, value);
    } else {
        constructed_key = map->key_base->allocable->constructor(key, map->allocator);
        constructed_val = map->val_base->constructor(value, map->allocator);
        return sstk_rbtree_put(cur->data.tree, constructed_key, constructed_val);
    }
}
