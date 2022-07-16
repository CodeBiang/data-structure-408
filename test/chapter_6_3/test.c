#include <stdio.h>

#include "al_graph.h"
#define INF -1

// clang-format off
// 1 -> 2 -> 4
//   ↘ ↑ ↙ ↑  ↘
//      3 -> 5 -> 6
// 结果为 1 2 3  4 5 6
int adj_matrix[] = {
    0,   1,   1,   INF, INF, INF, 
    INF, 0,   INF, 1,   INF, INF,
    INF, 1,   0,   INF, 1,   INF, 
    INF, INF, 1,   0,   INF, 1,
    INF, INF, INF, 1,   0,   1, 
    INF, INF, INF, INF, INF, 0
};

// ┌─── 1 ─── 7 
// │   / \    │
// │  2   3   │
// │ ┌─ 4 ──┐ │
// 6 ┴──────┴ 5
int adj_matrix1[] = {
    /*
    1    2    3    4    5    6    7
    */
    0,   1,   1,   INF, INF, 1,   1,
    1,   0,   INF, INF, INF, INF, INF,
    1,   INF,   0, INF, INF, INF, INF,
    INF, INF, INF, 0,   1,   1,   INF,
    INF, INF, INF, 1,   0,   1,   1,
    1,   INF, INF, 1,   1,   0,   INF,
    1,   INF, INF, INF, 1,   INF, 0
};
// clang-format on

static bool int_equals(const basic_obj_t* a, const basic_obj_t* b) {
    return a->i == b->i;
}

static void init_by_adj_matrix(al_graph_t* g, int* matrix, int n) {
    al_graph_init(g, int_equals);
    for (int i = 0; i < n; i++) {
        al_graph_insert_vertex(g)->data.i = i + 1;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i * n + j] == 0 || matrix[i * n + j] == INF) continue;
            al_graph_add_arc(g, basic_int_wrap(i + 1), basic_int_wrap(j + 1))
                ->data.i = matrix[i + j];
        }
    }
}

static void vertex_print(vert_node_t* vert, void* arg) {
    printf("%d \n", vert->data.i);
}

int main() {
    al_graph_t g;
    init_by_adj_matrix(&g, adj_matrix, 6);
    // 测试bfs用例为有向图
    al_graph_traverse_bfs(&g, vertex_print, NULL);
    printf("\n");

    al_graph_t g1;
    init_by_adj_matrix(&g1, adj_matrix1, 7);
    // 测试dfs用例为无向图
    al_graph_traverse_dfs(&g1, vertex_print, NULL);
    printf("\n");
    return 0;
}
