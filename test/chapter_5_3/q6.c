#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_stack.h"

bitree_t* tree_build(int pre[], int in[], int pre_start, int pre_end,
                     int in_start, int in_end) {
    if (in_start > in_end || pre_start > pre_end) return NULL;

    binode_t* node = binode_create();
    node->data.i = pre[pre_start];
    int cur_idx;
    int llen, rlen;

    for (cur_idx = in_start; cur_idx <= in_end && pre[pre_start] != in[cur_idx];
         cur_idx++)
        ;
    llen = cur_idx - in_start;
    rlen = in_end - cur_idx;
    
    node->l =
        tree_build(pre, in, pre_start + 1, pre_start + llen, in_start, cur_idx - 1);

    node->r =
        tree_build(pre, in, pre_end - rlen + 1, pre_end, cur_idx + 1, in_end);
    return node;
}

static void node_print(binode_t* node, void* arg) { printf("%d ", node->data.i); }

int main() {
    //       A
    //   B       C
    //     D   E
    //    F
    // 先序 A B D C E
    //     2 4 1 5 3
    // 中序 B D A E C
    int pre[] = {1, 2, 4, 3, 5};
    int in[] = {2, 4, 1, 5, 3};
    bitree_t* tree = tree_build(pre, in, 0, 4, 0, 4);
    bitree_in_traverse(tree, node_print, NULL);
    printf("\n");
    return 0;
}
