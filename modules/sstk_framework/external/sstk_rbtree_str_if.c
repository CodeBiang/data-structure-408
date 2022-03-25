#include <sstk_core.h>

#include <sstk_key_common.h>
#include <sstk_rbtree_if.h>


struct sstk_hash_str_s {
    int hash;
    sstk_str_t str;
};

static int sstk_str_rbtree_put(sstk_rbtree_str_if_t* tree, sstk_str_t* key, void* val);
static void sstk_str_rbtree_remove(sstk_rbtree_str_if_t* tree, sstk_str_t* key);
static const sstk_rbnode_t* sstk_str_rbtree_get(sstk_rbtree_str_if_t* tree, sstk_str_t* key, void** pval);
static sstk_rbtree_str_if_t sstk_str_rbtree_if = {
    .put = sstk_str_rbtree_put,
    .remove = sstk_str_rbtree_remove,
    .get = sstk_str_rbtree_get
};


int sstk_str_rbtree_init(sstk_rbtree_t* tree, sstk_allocable_t* val_allocatable, sstk_allocator_t* allocator) {
    sstk_memcpy(&tree->tree_if, &sstk_str_rbtree_if, sizeof(sstk_rbtree_if_t));
    
    return sstk_rbtree_init(tree, &sstk_str_key_base, val_allocatable, allocator);
}


int sstk_str_rbtree_put(sstk_rbtree_str_if_t* tree, sstk_str_t* key, void* val) {
    return sstk_rbtree_put((sstk_rbtree_t*) tree, key, val);
}

void sstk_str_rbtree_remove(sstk_rbtree_str_if_t* tree, sstk_str_t* key) {
    struct sstk_hash_str_s s = {
        .hash = 0,
        .str = {
            .data = key->data,
            .len = key->len
        }
    };
    return sstk_rbtree_remove((sstk_rbtree_t*) tree, &s);
}

const sstk_rbnode_t* sstk_str_rbtree_get(sstk_rbtree_str_if_t* tree, sstk_str_t* key, void** pval) {
    struct sstk_hash_str_s s = {
        .hash = 0,
        .str = {
            .data = key->data,
            .len = key->len
        }
    };
    return sstk_rbtree_get((sstk_rbtree_t*) tree, &s, pval);
}
