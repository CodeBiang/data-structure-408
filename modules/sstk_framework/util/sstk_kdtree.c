#include <math.h>
#include <sstk_core.h>
#include <sstk_kdtree.h>


#if SSTK_THREADS
#define USE_LIST_NODE_ALLOCATOR
#endif

typedef struct kdhyperrect_s kdhyperrect_t;
typedef struct kdnode_s kdnode_t;
typedef struct res_node_s res_node_t;

struct kdhyperrect_s {
    int dim;

    double* min;
    double* max; /* minimum/maximum coords */
};

struct kdnode_s {
    double* pos;
    int dir;
    void* data;

    kdnode_t* left;
    kdnode_t* right; /* negative/positive side */
};

struct res_node_s {
    kdnode_t* item;
    double dist_sq;
    res_node_t* next;
};

struct kdtree_s {
    int dim;
    
    kdnode_t* root;
    kdhyperrect_t* rect;
    
    sstk_allocator_t* allocator;
    sstk_allocable_t* allocable;
};

struct kdres_s {
    sstk_kdtree_t* tree;
    res_node_t* rlist;
    res_node_t* riter;
    int size;
};

#define SQ(x) ((x) * (x))
#define kd_construct(tree, ptr) tree->allocable->constructor(ptr, tree->allocator)
#define kd_destruct(tree, ptr) tree->allocable->destructor(ptr, tree->allocator)

static void clear_rec(kdnode_t* node, sstk_kdtree_t* tree);
static int insert_rec(kdnode_t** node, const double* pos, void* data, int dir, int dim);
static int rlist_insert(res_node_t* list, kdnode_t* item, double dist_sq);
static void clear_results(sstk_kdres_t* set);

static kdhyperrect_t* hyperrect_create(int dim, const double* min, const double* max);
static void hyperrect_free(kdhyperrect_t* rect);
static kdhyperrect_t* hyperrect_duplicate(const kdhyperrect_t* rect);
static void hyperrect_extend(kdhyperrect_t* rect, const double* pos);
static double hyperrect_dist_sq(kdhyperrect_t* rect, const double* pos);

#ifdef USE_LIST_NODE_ALLOCATOR
static res_node_t* alloc_resnode(void);
static void free_resnode(res_node_t*);
#else
#define alloc_resnode() malloc(sizeof(res_node_t))
#define free_resnode(n) free(n)
#endif

sstk_kdtree_t* sstk_kd_create(const sstk_kdtree_opt_t* opt) {
    struct kdtree_s* tree;
    sstk_allocator_t* allocator;
    sstk_allocable_t* allocable;

    allocator = opt->allocator ? opt->allocator : sstk_default_allocator;
    allocable = opt->allocable ? opt->allocable : &sstk_default_allocable;

    if (!(tree = allocator->alloc_fn(sizeof *tree))) {
        return 0;
    }

    tree->dim = opt->dim;
    tree->root = NULL;
    tree->rect = NULL;
    tree->allocable = opt->allocable;
    tree->allocator = opt->allocator;

    return tree;
}

void sstk_kd_free(sstk_kdtree_t* tree) {
    if (tree) {
        sstk_kd_clear(tree);
        free(tree);
    }
}

static void clear_rec(kdnode_t* node, sstk_kdtree_t* tree) {
    if (!node) return;

    clear_rec(node->left, tree);
    clear_rec(node->right, tree);

    tree->allocable->destructor(node->data, tree->allocator);

    free(node->pos);
    free(node);
}

void sstk_kd_clear(sstk_kdtree_t* tree) {
    clear_rec(tree->root, tree);
    tree->root = 0;

    if (tree->rect) {
        hyperrect_free(tree->rect);
        tree->rect = 0;
    }
}

static int insert_rec(kdnode_t** nptr, const double* pos, void* data, int dir, int dim) {
    int new_dir;
    kdnode_t* node;

    if (!*nptr) {
        if (!(node = malloc(sizeof *node))) {
            return -1;
        }
        if (!(node->pos = malloc(dim * sizeof *node->pos))) {
            free(node);
            return -1;
        }
        memcpy(node->pos, pos, dim * sizeof *node->pos);
        node->data = data;
        node->dir = dir;
        node->left = node->right = 0;
        *nptr = node;
        return 0;
    }

    node = *nptr;
    new_dir = (node->dir + 1) % dim;
    if (pos[node->dir] < node->pos[node->dir]) {
        return insert_rec(&(*nptr)->left, pos, data, new_dir, dim);
    }
    return insert_rec(&(*nptr)->right, pos, data, new_dir, dim);
}

int sstk_kd_insert(sstk_kdtree_t* tree, const double* pos, void* data) {
    
    if (insert_rec(&tree->root, pos, data, 0, tree->dim)) {
        return -1;
    }

    if (tree->rect == 0) {
        tree->rect = hyperrect_create(tree->dim, pos, pos);
    } else {
        hyperrect_extend(tree->rect, pos);
    }

    return 0;
}

int sstk_kd_insertf(sstk_kdtree_t* tree, const float* pos, void* data) {
    static double sbuf[16];
    double *bptr, *buf = 0;
    int res, dim = tree->dim;

    if (dim > 16) {
#ifndef NO_ALLOCA
        if (dim <= 256)
            bptr = buf = alloca(dim * sizeof *bptr);
        else
#endif
            if (!(bptr = buf = malloc(dim * sizeof *bptr))) {
            return -1;
        }
    } else {
        bptr = buf = sbuf;
    }

    while (dim-- > 0) {
        *bptr++ = *pos++;
    }

    res = sstk_kd_insert(tree, buf, data);
#ifndef NO_ALLOCA
    if (tree->dim > 256)
#else
    if (tree->dim > 16)
#endif
        free(buf);
    return res;
}

int sstk_kd_insert3(sstk_kdtree_t* tree, double x, double y, double z, void* data) {
    double buf[3];
    buf[0] = x;
    buf[1] = y;
    buf[2] = z;
    return sstk_kd_insert(tree, buf, data);
}

int sstk_kd_insert3f(sstk_kdtree_t* tree, float x, float y, float z, void* data) {
    double buf[3];
    buf[0] = x;
    buf[1] = y;
    buf[2] = z;
    return sstk_kd_insert(tree, buf, data);
}

static int find_nearest(kdnode_t* node, const double* pos, double range, res_node_t* list,
                        int ordered, int dim) {
    double dist_sq, dx;
    int i, ret, added_res = 0;

    if (!node) return 0;

    dist_sq = 0;
    for (i = 0; i < dim; i++) {
        dist_sq += SQ(node->pos[i] - pos[i]);
    }
    if (dist_sq <= SQ(range)) {
        if (rlist_insert(list, node, ordered ? dist_sq : -1.0) == -1) {
            return -1;
        }
        added_res = 1;
    }

    dx = pos[node->dir] - node->pos[node->dir];

    ret = find_nearest(dx <= 0.0 ? node->left : node->right, pos, range, list, ordered, dim);
    if (ret >= 0 && fabs(dx) < range) {
        added_res += ret;
        ret = find_nearest(dx <= 0.0 ? node->right : node->left, pos, range, list, ordered, dim);
    }
    if (ret == -1) {
        return -1;
    }
    added_res += ret;

    return added_res;
}

#if 0
static int find_nearest_n(kdnode_t* node, const double* pos, double range, int num, struct rheap *heap, int dim)
{
	double dist_sq, dx;
	int i, ret, added_res = 0;

	if(!node) return 0;
	
	/* if the photon is close enough, add it to the result heap */
	dist_sq = 0;
	for(i=0; i<dim; i++) {
		dist_sq += SQ(node->pos[i] - pos[i]);
	}
	if(dist_sq <= range_sq) {
		if(heap->size >= num) {
			/* get furthest element */
			res_node_t* maxelem = rheap_get_max(heap);

			/* and check if the new one is closer than that */
			if(maxelem->dist_sq > dist_sq) {
				rheap_remove_max(heap);

				if(rheap_insert(heap, node, dist_sq) == -1) {
					return -1;
				}
				added_res = 1;

				range_sq = dist_sq;
			}
		} else {
			if(rheap_insert(heap, node, dist_sq) == -1) {
				return =1;
			}
			added_res = 1;
		}
	}


	/* find signed distance from the splitting plane */
	dx = pos[node->dir] - node->pos[node->dir];

	ret = find_nearest_n(dx <= 0.0 ? node->left : node->right, pos, range, num, heap, dim);
	if(ret >= 0 && fabs(dx) < range) {
		added_res += ret;
		ret = find_nearest_n(dx <= 0.0 ? node->right : node->left, pos, range, num, heap, dim);
	}

}
#endif

static void sstk_kd_nearest_i(kdnode_t* node, const double* pos, kdnode_t** result,
                         double* result_dist_sq, kdhyperrect_t* rect) {
    int dir = node->dir;
    int i;
    double dummy, dist_sq;
    kdnode_t *nearer_subtree, *farther_subtree;
    double *nearer_hyperrect_coord, *farther_hyperrect_coord;

    /* Decide whether to go left or right in the tree */
    dummy = pos[dir] - node->pos[dir];
    if (dummy <= 0) {
        nearer_subtree = node->left;
        farther_subtree = node->right;
        nearer_hyperrect_coord = rect->max + dir;
        farther_hyperrect_coord = rect->min + dir;
    } else {
        nearer_subtree = node->right;
        farther_subtree = node->left;
        nearer_hyperrect_coord = rect->min + dir;
        farther_hyperrect_coord = rect->max + dir;
    }

    if (nearer_subtree) {
        /* Slice the hyperrect to get the hyperrect of the nearer subtree */
        dummy = *nearer_hyperrect_coord;
        *nearer_hyperrect_coord = node->pos[dir];
        /* Recurse down into nearer subtree */
        sstk_kd_nearest_i(nearer_subtree, pos, result, result_dist_sq, rect);
        /* Undo the slice */
        *nearer_hyperrect_coord = dummy;
    }

    /* Check the distance of the point at the current node, compare it
     * with our best so far */
    dist_sq = 0;
    for (i = 0; i < rect->dim; i++) {
        dist_sq += SQ(node->pos[i] - pos[i]);
    }
    if (dist_sq < *result_dist_sq) {
        *result = node;
        *result_dist_sq = dist_sq;
    }

    if (farther_subtree) {
        /* Get the hyperrect of the farther subtree */
        dummy = *farther_hyperrect_coord;
        *farther_hyperrect_coord = node->pos[dir];
        /* Check if we have to recurse down by calculating the closest
         * point of the hyperrect and see if it's closer than our
         * minimum distance in result_dist_sq. */
        if (hyperrect_dist_sq(rect, pos) < *result_dist_sq) {
            /* Recurse down into farther subtree */
            sstk_kd_nearest_i(farther_subtree, pos, result, result_dist_sq, rect);
        }
        /* Undo the slice on the hyperrect */
        *farther_hyperrect_coord = dummy;
    }
}

sstk_kdres_t* sstk_kd_nearest(sstk_kdtree_t* tree, const double* pos) {
    struct kdtree_s* kd = (struct kdtree_s*) tree;
    kdhyperrect_t* rect;
    kdnode_t* result;
    struct kdres_s* rset;
    double dist_sq;
    int i;

    if (!kd) return 0;
    if (!kd->rect) return 0;

    /* Allocate result set */
    if (!(rset = malloc(sizeof *rset))) {
        return 0;
    }
    if (!(rset->rlist = alloc_resnode())) {
        free(rset);
        return 0;
    }
    rset->rlist->next = 0;
    rset->tree = kd;

    /* Duplicate the bounding hyperrectangle, we will work on the copy */
    if (!(rect = hyperrect_duplicate(kd->rect))) {
        sstk_kd_res_free(rset);
        return 0;
    }

    /* Our first guesstimate is the root node */
    result = kd->root;
    dist_sq = 0;
    for (i = 0; i < kd->dim; i++) dist_sq += SQ(result->pos[i] - pos[i]);

    /* Search for the nearest neighbour recursively */
    sstk_kd_nearest_i(kd->root, pos, &result, &dist_sq, rect);

    /* Free the copy of the hyperrect */
    hyperrect_free(rect);

    /* Store the result */
    if (result) {
        if (rlist_insert(rset->rlist, result, -1.0) == -1) {
            sstk_kd_res_free(rset);
            return 0;
        }
        rset->size = 1;
        sstk_kd_res_rewind(rset);
        return rset;
    } else {
        sstk_kd_res_free(rset);
        return 0;
    }
}

sstk_kdres_t* sstk_kd_nearestf(sstk_kdtree_t* tree, const float* pos) {
    static double sbuf[16];
    double *bptr, *buf = 0;
    int dim = tree->dim;
    sstk_kdres_t* res;

    if (dim > 16) {
#ifndef NO_ALLOCA
        if (dim <= 256)
            bptr = buf = alloca(dim * sizeof *bptr);
        else
#endif
            if (!(bptr = buf = malloc(dim * sizeof *bptr))) {
            return 0;
        }
    } else {
        bptr = buf = sbuf;
    }

    while (dim-- > 0) {
        *bptr++ = *pos++;
    }

    res = sstk_kd_nearest(tree, buf);
#ifndef NO_ALLOCA
    if (tree->dim > 256)
#else
    if (tree->dim > 16)
#endif
        free(buf);
    return res;
}

sstk_kdres_t* sstk_kd_nearest3(sstk_kdtree_t* tree, double x, double y, double z) {
    double pos[3];
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    return sstk_kd_nearest(tree, pos);
}

sstk_kdres_t* sstk_kd_nearest3f(sstk_kdtree_t* tree, float x, float y, float z) {
    double pos[3];
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    return sstk_kd_nearest(tree, pos);
}

/* ---- nearest N search ---- */
/*
static kdres *sstk_kd_nearest_n(sstk_kdtree_t* kd, const double* pos, int num)
{
        int ret;
        struct kdres *rset;

        if(!(rset = malloc(sizeof *rset))) {
                return 0;
        }
        if(!(rset->rlist = alloc_resnode())) {
                free(rset);
                return 0;
        }
        rset->rlist->next = 0;
        rset->tree = kd;

        if((ret = find_nearest_n(kd->root, pos, range, num, rset->rlist, kd->dim)) == -1) {
                sstk_kd_res_free(rset);
                return 0;
        }
        rset->size = ret;
        sstk_kd_res_rewind(rset);
        return rset;
}*/

static sstk_kdres_t* _sstk_kd_nearest_range(sstk_kdtree_t* kd, const double* pos, double range, int ordered) {
    int ret;
    struct kdres_s* rset;

    if (!(rset = malloc(sizeof *rset))) {
        return 0;
    }
    if (!(rset->rlist = alloc_resnode())) {
        free(rset);
        return 0;
    }
    rset->rlist->next = 0;
    rset->tree = kd;

    if ((ret = find_nearest(kd->root, pos, range, rset->rlist, ordered, kd->dim)) == -1) {
        sstk_kd_res_free(rset);
        return 0;
    }
    rset->size = ret;
    sstk_kd_res_rewind(rset);
    return rset;
}

sstk_kdres_t* _sstk_kd_nearest_rangef(sstk_kdtree_t* kd, const float* pos, float range, int ordered) {
    static double sbuf[16];
    double *bptr, *buf = 0;
    int dim = kd->dim;
    sstk_kdres_t* res;

    if (dim > 16) {
#ifndef NO_ALLOCA
        if (dim <= 256)
            bptr = buf = alloca(dim * sizeof *bptr);
        else
#endif
            if (!(bptr = buf = malloc(dim * sizeof *bptr))) {
            return 0;
        }
    } else {
        bptr = buf = sbuf;
    }

    while (dim-- > 0) {
        *bptr++ = *pos++;
    }

    res = _sstk_kd_nearest_range(kd, buf, range, ordered);
#ifndef NO_ALLOCA
    if (kd->dim > 256)
#else
    if (kd->dim > 16)
#endif
        free(buf);
    return res;
}

sstk_kdres_t* _sstk_kd_nearest_range3(sstk_kdtree_t* tree, double x, double y, double z, double range, int ordered) {
    double buf[3];
    buf[0] = x;
    buf[1] = y;
    buf[2] = z;
    return _sstk_kd_nearest_range(tree, buf, range, ordered);
}

sstk_kdres_t* _sstk_kd_nearest_range3f(sstk_kdtree_t* tree, float x, float y, float z, float range, int ordered) {
    double buf[3];
    buf[0] = x;
    buf[1] = y;
    buf[2] = z;
    return _sstk_kd_nearest_range(tree, buf, range, ordered);
}

sstk_kdres_t* sstk_kd_nearest_range(sstk_kdtree_t* tree, const double* pos, double range) {
    return _sstk_kd_nearest_range(tree, pos, range, 0);
}

sstk_kdres_t* sstk_kd_nearest_rangef(sstk_kdtree_t* tree, const float* pos, float range) {
    return _sstk_kd_nearest_rangef(tree, pos, range, 0);
}

sstk_kdres_t* sstk_kd_nearest_range3(sstk_kdtree_t* tree, double x, double y, double z, double range) {
    return _sstk_kd_nearest_range3(tree, x, y, z, range, 0);
}

sstk_kdres_t* sstk_kd_nearest_range3f(sstk_kdtree_t* tree, float x, float y, float z, float range) {
    return _sstk_kd_nearest_range3f(tree, x, y, z, range, 0);
}


sstk_kdres_t* sstk_kd_nearest_range_ordered(sstk_kdtree_t* tree, const double* pos, double range) {
    return _sstk_kd_nearest_range(tree, pos, range, 1);
}

sstk_kdres_t* sstk_kd_nearest_rangef_ordered(sstk_kdtree_t* tree, const float* pos, float range) {
    return _sstk_kd_nearest_rangef(tree, pos, range, 1);
}

sstk_kdres_t* sstk_kd_nearest_range3_ordered(sstk_kdtree_t* tree, double x, double y, double z, double range) {
    return _sstk_kd_nearest_range3(tree, x, y, z, range, 1);
}

sstk_kdres_t* sstk_kd_nearest_range3f_ordered(sstk_kdtree_t* tree, float x, float y, float z, float range) {
    return _sstk_kd_nearest_range3f(tree, x, y, z, range, 1);
}


void sstk_kd_res_free(sstk_kdres_t* rset) {
    
    clear_results(rset);
    free_resnode(rset->rlist);
    free(rset);
}

int sstk_kd_res_size(struct kdres_s* set) { return (set->size); }

void sstk_kd_res_rewind(struct kdres_s* rset) { rset->riter = rset->rlist->next; }

sstk_bool_t sstk_kd_res_end(struct kdres_s* rset) { return rset->riter == 0; }
sstk_bool_t sstk_kd_res_has_next(struct kdres_s* rset) { return rset->riter != 0; }

sstk_bool_t sstk_kd_res_next(struct kdres_s* rset) {
    rset->riter = rset->riter->next;
    return rset->riter != 0;
}

void* sstk_kd_res_item(struct kdres_s* rset, double* pos) {
    if (rset->riter) {
        if (pos) {
            memcpy(pos, rset->riter->item->pos, rset->tree->dim * sizeof *pos);
        }
        return rset->riter->item->data;
    }
    return 0;
}

void* sstk_kd_res_itemf(sstk_kdres_t* rset, float* pos) {
    if (rset->riter) {
        if (pos) {
            int i;
            for (i = 0; i < rset->tree->dim; i++) {
                pos[i] = rset->riter->item->pos[i];
            }
        }
        return rset->riter->item->data;
    }
    return 0;
}

void* sstk_kd_res_item3(sstk_kdres_t* rset, double* x, double* y, double* z) {
    if (rset->riter) {
        if (x) *x = rset->riter->item->pos[0];
        if (y) *y = rset->riter->item->pos[1];
        if (z) *z = rset->riter->item->pos[2];
        return rset->riter->item->data;
    }
    return 0;
}

void* sstk_kd_res_item3f(sstk_kdres_t* rset, float* x, float* y, float* z) {
    if (rset->riter) {
        if (x) *x = rset->riter->item->pos[0];
        if (y) *y = rset->riter->item->pos[1];
        if (z) *z = rset->riter->item->pos[2];
        return rset->riter->item->data;
    }
    return 0;
}

void* sstk_kd_res_item_data(sstk_kdres_t* set) { return sstk_kd_res_item(set, 0); }

/* ---- hyperrectangle helpers ---- */
static kdhyperrect_t* hyperrect_create(int dim, const double* min, const double* max) {
    size_t size = dim * sizeof(double);
    kdhyperrect_t* rect = 0;

    if (!(rect = malloc(sizeof(kdhyperrect_t)))) {
        return 0;
    }

    rect->dim = dim;
    if (!(rect->min = malloc(size))) {
        free(rect);
        return 0;
    }
    if (!(rect->max = malloc(size))) {
        free(rect->min);
        free(rect);
        return 0;
    }
    memcpy(rect->min, min, size);
    memcpy(rect->max, max, size);

    return rect;
}

static void hyperrect_free(kdhyperrect_t* rect) {
    free(rect->min);
    free(rect->max);
    free(rect);
}

static kdhyperrect_t* hyperrect_duplicate(const kdhyperrect_t* rect) {
    return hyperrect_create(rect->dim, rect->min, rect->max);
}

static void hyperrect_extend(kdhyperrect_t* rect, const double* pos) {
    int i;

    for (i = 0; i < rect->dim; i++) {
        if (pos[i] < rect->min[i]) {
            rect->min[i] = pos[i];
        }
        if (pos[i] > rect->max[i]) {
            rect->max[i] = pos[i];
        }
    }
}

static double hyperrect_dist_sq(kdhyperrect_t* rect, const double* pos) {
    int i;
    double result = 0;

    for (i = 0; i < rect->dim; i++) {
        if (pos[i] < rect->min[i]) {
            result += SQ(rect->min[i] - pos[i]);
        } else if (pos[i] > rect->max[i]) {
            result += SQ(rect->max[i] - pos[i]);
        }
    }

    return result;
}

/* ---- static helpers ---- */

#ifdef USE_LIST_NODE_ALLOCATOR
/* special list node allocators. */
static res_node_t* free_nodes;

#ifndef NO_PTHREADS
static pthread_mutex_t alloc_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static res_node_t* alloc_resnode(void) {
    res_node_t* node;

#ifndef NO_PTHREADS
    pthread_mutex_lock(&alloc_mutex);
#endif

    if (!free_nodes) {
        node = malloc(sizeof *node);
    } else {
        node = free_nodes;
        free_nodes = free_nodes->next;
        node->next = 0;
    }

#ifndef NO_PTHREADS
    pthread_mutex_unlock(&alloc_mutex);
#endif

    return node;
}

static void free_resnode(res_node_t* node) {
#ifndef NO_PTHREADS
    pthread_mutex_lock(&alloc_mutex);
#endif

    node->next = free_nodes;
    free_nodes = node;

#ifndef NO_PTHREADS
    pthread_mutex_unlock(&alloc_mutex);
#endif
}
#endif /* list node allocator or not */

/* inserts the item. if dist_sq is >= 0, then do an ordered insert */
/* TODO make the ordering code use heapsort */
static int rlist_insert(res_node_t* list, kdnode_t* item, double dist_sq) {
    res_node_t* rnode;

    if (!(rnode = alloc_resnode())) {
        return -1;
    }
    rnode->item = item;
    rnode->dist_sq = dist_sq;

    if (dist_sq >= 0.0) {
        while (list->next && list->next->dist_sq < dist_sq) {
            list = list->next;
        }
    }
    rnode->next = list->next;
    list->next = rnode;
    return 0;
}

static void clear_results(sstk_kdres_t* rset) {
    res_node_t *tmp, *node = rset->rlist->next;

    while (node) {
        tmp = node;
        node = node->next;
        free_resnode(tmp);
    }

    rset->rlist->next = 0;
}
