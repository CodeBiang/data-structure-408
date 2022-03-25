#include <sstk_core.h>

#define sstk_rbt_red(node) ((node)->color = 1)
#define sstk_rbt_black(node) ((node)->color = 0)
#define sstk_rbt_is_red(node) ((node)->color)
#define sstk_rbt_is_black(node) (!sstk_rbt_is_red(node))
#define sstk_rbt_copy_color(n1, n2) (n1->color = n2->color)

/*
 *     [y]                                            [x]
 *    /   \     --- right_rotate(tree, y) --->       /   \
 *  [x]    c                                        a    [y]
 *  / \         <--- left_rotate(tree, x) ----           / \
 * a   b                                                b   c
 */
static inline void sstk_rbtree_left_rotate(sstk_rbtree_t* tree, sstk_rbnode_t* node);
static inline void sstk_rbtree_right_rotate(sstk_rbtree_t* tree, sstk_rbnode_t* node);

int sstk_rbtree_init(sstk_rbtree_t* tree, sstk_key_base_t* key_base, sstk_allocable_t* val_base,
                     sstk_allocator_t* allocator) {
    sstk_rbnode_t* nil;
    sstk_allocator_t* a = allocator == NULL ? sstk_cycle->allocator : allocator;

    nil = a->alloc_fn(sizeof(sstk_entry_node_t));
    if (nil == NULL) return SSTK_ERR;
    sstk_rbt_black(nil);
    nil->parent = nil->left = nil->right = NULL;

    tree->root = tree->nil = nil;
    tree->allocator = a;
    tree->key_base = key_base ? key_base : &sstk_default_key_base;
    tree->val_base = val_base ? val_base : &sstk_default_allocable;
    tree->size = 0;

    return SSTK_OK;
}

int sstk_rbtree_insert_value(sstk_rbtree_t* tree, sstk_rbnode_t* node) {
    sstk_rbnode_t** p;
    sstk_rbnode_t* temp = tree->root;
    int cmp_ret;

    for (;;) {
        cmp_ret = tree->key_base->cmp(node->key, temp->key);

        if (cmp_ret == 0) {
            // 重复替换
            // 释放node key, temp val
            tree->key_base->allocable->destructor(node->key, tree->allocator);
            tree->val_base->destructor(temp->data, tree->allocator);
            temp->data = node->data;
            return 1;
        }

        p = (cmp_ret < 0) ? &temp->left : &temp->right;

        if (*p == tree->nil) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = tree->nil;
    node->right = tree->nil;
    sstk_rbt_red(node);

    return 0;
}

int sstk_rbtree_put_node(sstk_rbtree_t* tree, sstk_rbnode_t* node) {
    sstk_rbnode_t* nil;
    sstk_rbnode_t** root;
    sstk_rbnode_t* temp;

    root = &tree->root;
    nil = tree->nil;

    // 空树
    if (*root == nil) {
        node->left = nil;
        node->right = nil;
        node->parent = NULL;
        sstk_rbt_black(node);
        *root = node;
        tree->size = 1;
        return SSTK_OK;
    }

    // 插入值
    if (sstk_rbtree_insert_value(tree, node) == 1) {
        // 重复替换
        return SSTK_OK;
    } else {
        // 非相同key
        tree->size++;
    }

    // 重平衡
    while (node != *root && sstk_rbt_is_red(node->parent)) {
        if (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            if (sstk_rbt_is_red(temp)) {
                sstk_rbt_black(node->parent);
                sstk_rbt_black(temp);
                sstk_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    sstk_rbtree_left_rotate(tree, node);
                }

                sstk_rbt_black(node->parent);
                sstk_rbt_red(node->parent->parent);
                sstk_rbtree_right_rotate(tree, node->parent->parent);
            }

        } else {
            temp = node->parent->parent->left;

            if (sstk_rbt_is_red(temp)) {
                sstk_rbt_black(node->parent);
                sstk_rbt_black(temp);
                sstk_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    sstk_rbtree_right_rotate(tree, node);
                }

                sstk_rbt_black(node->parent);
                sstk_rbt_red(node->parent->parent);
                sstk_rbtree_left_rotate(tree, node->parent->parent);
            }
        }
    }

    sstk_rbt_black(*root);
    return SSTK_OK;
}

int sstk_rbtree_put(sstk_rbtree_t* tree, void* key, void* value) {
    sstk_rbnode_t* node;
    // 构造节点
    node = (sstk_rbnode_t*) tree->allocator->alloc_fn(sizeof(sstk_entry_node_t));
    node->type = SSTK_RBNODE_TYPE_CODE;
    node->key = tree->key_base->allocable->constructor(key, tree->allocator);
    node->data = tree->val_base->constructor(value, tree->allocator);

    return sstk_rbtree_put_node(tree, node);
}

static void sstk_rbnode_free(sstk_rbtree_t* tree, sstk_rbnode_t* node) {
    tree->key_base->allocable->destructor(node->key, tree->allocator);
    tree->val_base->destructor(node->data, tree->allocator);

    tree->allocator->dealloc_fn(node);
}

void sstk_rbtree_remove_node(sstk_rbtree_t* tree, sstk_rbnode_t* node) {
    sstk_uint_t red;
    sstk_rbnode_t** root;
    sstk_rbnode_t* nil;
    sstk_rbnode_t* subst;
    sstk_rbnode_t* temp;
    sstk_rbnode_t* w;

    // a binary tree delete
    root = &tree->root;
    nil = tree->nil;

    if (node->left == nil) {
        temp = node->right;
        subst = node;

    } else if (node->right == nil) {
        temp = node->left;
        subst = node;

    } else {
        subst = sstk_rbtree_min(node->right, nil);
        temp = subst->right;
    }

    if (subst == *root) {
        *root = temp;
        sstk_rbt_black(temp);

        // free node
        sstk_rbnode_free(tree, node);
        tree->size--;

        return;
    }

    red = sstk_rbt_is_red(subst);

    if (subst == subst->parent->left) {
        subst->parent->left = temp;

    } else {
        subst->parent->right = temp;
    }

    if (subst == node) {
        temp->parent = subst->parent;

    } else {
        if (subst->parent == node) {
            temp->parent = subst;

        } else {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        sstk_rbt_copy_color(subst, node);

        if (node == *root) {
            *root = subst;

        } else {
            if (node == node->parent->left) {
                node->parent->left = subst;
            } else {
                node->parent->right = subst;
            }
        }

        if (subst->left != nil) {
            subst->left->parent = subst;
        }

        if (subst->right != nil) {
            subst->right->parent = subst;
        }
    }

    // free node
    sstk_rbnode_free(tree, node);
    tree->size--;

    if (red) {
        return;
    }

    // a delete fixup
    while (temp != *root && sstk_rbt_is_black(temp)) {
        if (temp == temp->parent->left) {
            w = temp->parent->right;

            if (sstk_rbt_is_red(w)) {
                sstk_rbt_black(w);
                sstk_rbt_red(temp->parent);
                sstk_rbtree_left_rotate(tree, temp->parent);
                w = temp->parent->right;
            }

            if (sstk_rbt_is_black(w->left) && sstk_rbt_is_black(w->right)) {
                sstk_rbt_red(w);
                temp = temp->parent;

            } else {
                if (sstk_rbt_is_black(w->right)) {
                    sstk_rbt_black(w->left);
                    sstk_rbt_red(w);
                    sstk_rbtree_right_rotate(tree, w);
                    w = temp->parent->right;
                }

                sstk_rbt_copy_color(w, temp->parent);
                sstk_rbt_black(temp->parent);
                sstk_rbt_black(w->right);
                sstk_rbtree_left_rotate(tree, temp->parent);
                temp = *root;
            }

        } else {
            w = temp->parent->left;

            if (sstk_rbt_is_red(w)) {
                sstk_rbt_black(w);
                sstk_rbt_red(temp->parent);
                sstk_rbtree_right_rotate(tree, temp->parent);
                w = temp->parent->left;
            }

            if (sstk_rbt_is_black(w->left) && sstk_rbt_is_black(w->right)) {
                sstk_rbt_red(w);
                temp = temp->parent;

            } else {
                if (sstk_rbt_is_black(w->left)) {
                    sstk_rbt_black(w->right);
                    sstk_rbt_red(w);
                    sstk_rbtree_left_rotate(tree, w);
                    w = temp->parent->left;
                }

                sstk_rbt_copy_color(w, temp->parent);
                sstk_rbt_black(temp->parent);
                sstk_rbt_black(w->left);
                sstk_rbtree_right_rotate(tree, temp->parent);
                temp = *root;
            }
        }
    }

    sstk_rbt_black(temp);
}

void sstk_rbtree_remove(sstk_rbtree_t* tree, void* key) {
    sstk_rbnode_t* node;

    sstk_rbtree_foreach(node, tree) {
        if (tree->key_base->cmp(node->key, key) == 0) {
            sstk_rbtree_remove_node(tree, node);
            return;
        }
    }
}

const sstk_rbnode_t* sstk_rbtree_get(sstk_rbtree_t* tree, void* key, void** data) {
    sstk_rbnode_t* node = tree->root;
    int cmp_ret;

    while (node != tree->nil) {
        cmp_ret = tree->key_base->cmp(key, node->key);
        if (cmp_ret == 0) {
            if (data) *data = node->data;

            return node;
        } else if (cmp_ret < 0) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    return NULL;
}

/*
 *     [y]                                            [x][node]
 *    /   \     --- right_rotate(tree, y) --->       /   \
 *  [x]    c                                        a    [y][temp]
 *  / \         <--- left_rotate(tree, x) ----           / \
 * a   b                                                b   c
 */
void sstk_rbtree_left_rotate(sstk_rbtree_t* tree, sstk_rbnode_t* node) {
    sstk_rbnode_t* temp = node->right;

    node->right = temp->left;
    // b非NIL节点更新父节点
    if (temp->left != tree->nil) temp->left->parent = node;

    temp->parent = node->parent;

    // 当x为root时, 需要切换树根
    if (node == tree->root) {
        tree->root = temp;
    } else if (node == node->parent->left) {
        // x不为树根, 判断y和其新父节点的大小
        node->parent->left = temp;
    } else {
        node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}

/*
 *     [y][node]                                      [x]
 *    /   \     --- right_rotate(tree, y) --->       /   \
 *  [x][t] c                                        a    [y]
 *  / \         <--- left_rotate(tree, x) ----           / \
 * a   b                                                b   c
 */
void sstk_rbtree_right_rotate(sstk_rbtree_t* tree, sstk_rbnode_t* node) {
    sstk_rbnode_t* temp = node->left;

    node->left = temp->right;
    // 非NIL节点更新父节点
    if (temp->right != tree->nil) temp->right->parent = node;

    temp->parent = node->parent;
    // 当x为root时, 需要切换树根
    if (node == tree->root) {
        tree->root = temp;
    } else if (node == node->parent->right) {
        // x不为树根, 判断y和其新父节点的大小
        node->parent->right = temp;
    } else {
        node->parent->left = temp;
    }

    temp->right = node;
    node->parent = temp;
}

sstk_rbnode_t* sstk_rbtree_next(sstk_rbtree_t* tree, sstk_rbnode_t* node) {
    sstk_rbnode_t *root, *nil, *parent;

    nil = tree->nil;

    if (node->right != nil) {
        return sstk_rbtree_min(node->right, nil);
    }

    root = tree->root;

    for (;;) {
        parent = node->parent;

        if (node == root) {
            return NULL;
        }

        if (node == parent->left) {
            return parent;
        }

        node = parent;
    }
}

void sstk_rbtree_clear(sstk_rbtree_t* tree) {
    sstk_rbnode_t* node;
    sstk_rbnode_t* nil = tree->nil;

    sstk_rbtree_foreach(node, tree) {
        sstk_rbnode_free(tree, node);
    }

    nil->parent = nil->left = nil->right = NULL;
    tree->root = tree->nil = nil;
    tree->size = 0;
}

void sstk_rbtree_free(sstk_rbtree_t* tree) {
    sstk_rbtree_clear(tree);
    tree->allocator->dealloc_fn(tree->nil);
}
