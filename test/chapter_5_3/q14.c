#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_queue.h"

typedef struct {
    binode_t* node;
    int level;
} level_node_t;

static level_node_t* level_node(binode_t* node, int level) {
    level_node_t* n = malloc(sizeof(level_node_t));
    n->level = level;
    n->node = node;
    return n;
}

#define level_node_cast(p) ((level_node_t*) (p))

#define MAX_LEVEL 50

int tree_width(binode_t* node) {
    if (!node) return 0;
    int max_width = 0;
    int level_cnt[MAX_LEVEL] = {0};
    linked_queue_t q;
    linked_queue_init(&q);
    linked_queue_enq(&q)->data.obj = level_node(node, 0);

    while (!linked_queue_empty(&q)) {
        linked_node_t* temp = linked_queue_deq(&q);
        level_node_t* cur = level_node_cast(temp->data.obj);

        // printf("%d - level %d\n", cur->node->data.i, cur->level);
        level_cnt[cur->level]++;
        if (level_cnt[cur->level] > max_width) {
            max_width = level_cnt[cur->level];
        }

        if (cur->node->l) {
            linked_queue_enq(&q)->data.obj =
                level_node(cur->node->l, cur->level + 1);
        }
        if (cur->node->r) {
            linked_queue_enq(&q)->data.obj =
                level_node(cur->node->r, cur->level + 1);
        }

        free(cur);
        free(temp);
    }

    return max_width;
}

int main() {
    //       1
    //   2       3
    //     4   5   6
    //              7
    bitree_t* root = binode_create();
    root->l = binode_create();
    root->data.i = 1;
    root->l = binode_create();
    root->l->data.i = 2;
    root->l->r = binode_create();
    root->l->r->data.i = 4;
    root->r = binode_create();
    root->r->data.i = 3;
    root->r->l = binode_create();
    root->r->l->data.i = 5;
    root->r->r = binode_create();
    root->r->r->data.i = 6;
    root->r->r->r = binode_create();
    root->r->r->r->data.i = 7;
    int w = tree_width(root);

    printf("width %d\n", w);
    return 0;
}
