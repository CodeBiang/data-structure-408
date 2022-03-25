#include <sstk_core.h>

#include <sstk_key_common.h>
#include <sstk_rbtree_if.h>

static int sstk_int_rbtree_put(sstk_rbtree_int_if_t* tree, int key, void* val);
static void sstk_int_rbtree_remove(sstk_rbtree_int_if_t* tree, int key);
static const sstk_rbnode_t* sstk_int_rbtree_get(sstk_rbtree_int_if_t* tree, int key, void** pval);
static sstk_rbtree_int_if_t sstk_int_rbtree_if = {
    .put = sstk_int_rbtree_put,
    .remove = sstk_int_rbtree_remove,
    .get = sstk_int_rbtree_get
};


int sstk_int_rbtree_init(sstk_rbtree_t* tree, sstk_allocable_t* val_allocatable, sstk_allocator_t* allocator) {
    sstk_memcpy(&tree->tree_if, &sstk_int_rbtree_if, sizeof(sstk_rbtree_if_t));
    
    return sstk_rbtree_init(tree, &sstk_int_key_base, val_allocatable, allocator);
}


int sstk_int_rbtree_put(sstk_rbtree_int_if_t* tree, int key, void* val) {
    return sstk_rbtree_put((sstk_rbtree_t*) tree, &key, val);
}

void sstk_int_rbtree_remove(sstk_rbtree_int_if_t* tree, int key) {
    sstk_rbtree_remove((sstk_rbtree_t*) tree, &key);
}

const sstk_rbnode_t* sstk_int_rbtree_get(sstk_rbtree_int_if_t* tree, int key, void** pval) {
    return sstk_rbtree_get((sstk_rbtree_t*) tree, &key, pval);
}
