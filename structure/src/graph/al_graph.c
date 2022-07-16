#include "al_graph.h"

#include <stdlib.h>
#include <string.h>

#include "sq_queue.h"

static bool memequals(const basic_obj_t* a, const basic_obj_t* b) {
    return memcmp(&a, &b, sizeof(basic_obj_t)) == 0;
}

void al_graph_init(al_graph_t* g, basic_obj_equals_fn item_eq) {
    memset(g, 0, sizeof(al_graph_t));
    g->vert_equals = item_eq == NULL ? memequals : item_eq;
}

bool al_graph_adjacent(const al_graph_t* g, const basic_obj_t* x,
                       const basic_obj_t* y) {
    arc_node_t* arc;
    vert_node_t* v = al_graph_get_vertex(g, x);

    if (v == NULL) return false;

    arc = v->first;

    while (arc) {
        if (g->vert_equals(&g->vertices[arc->vert].data, y)) {
            return true;
        }
        arc = arc->next;
    }

    return false;
}

void al_graph_neighbor_arcs_foreach(al_graph_t* g, const basic_obj_t* x,
                                    arc_node_accept_fn accept, void* arg) {
    vert_node_t* v = al_graph_get_vertex(g, x);
    if (v == NULL) return;

    arc_node_t* arc = v->first;

    while (arc) {
        accept(arc, arg);
        arc = arc->next;
    }
}

void al_graph_neighbor_vertices_foreach(al_graph_t* g, const basic_obj_t* x,
                                        vert_node_accept_fn accept, void* arg) {
    vert_node_t* v = al_graph_get_vertex(g, x);
    if (v == NULL) return;

    arc_node_t* arc = v->first;

    while (arc) {
        accept(&g->vertices[arc->vert], arg);
        arc = arc->next;
    }
}

vert_node_t* al_graph_insert_vertex(al_graph_t* g) {
    if (g->nvert >= AL_GRAPH_MAX_VERTICES) return NULL;
    return &g->vertices[g->nvert++];
}

bool al_graph_delete_vertex(al_graph_t* g, const basic_obj_t* x) {
    int v = -1;
    for (int i = 0; i < g->nvert; i++) {
        if (g->vert_equals(&g->vertices[i].data, x)) {
            v = i;
            break;
        }
    }

    if (v == -1) return false;

    // 内存移动
    memmove(&g->vertices[v], &g->vertices[v + 1],
            sizeof(vert_node_t) * (g->nvert - v - 1));
    g->nvert--;

    // 遍历删除与v邻接的边节点
    for (int i = 0; i < g->nvert; i++) {
        arc_node_t* a = g->vertices[i].first;

        if (a && a->vert == v) {
            g->vertices[i].first = g->vertices[i].first->next;
            // 释放弧节点
            free(a);
            // 断言 不存在重复弧节点, 所以跳出循环
            continue;
        }

        arc_node_t* pre = a;
        a = a->next;

        // 继续查找与v邻接的边并删除
        while (a) {
            if (a->vert == v) {
                pre->next = a->next;
                free(a);
                break;
            }
            a = a->next;
        }
    }

    return true;
}

arc_node_t* al_graph_add_arc(al_graph_t* g, const basic_obj_t* x,
                             const basic_obj_t* y) {
    vert_node_t* vx = al_graph_get_vertex(g, x);
    int idxy = al_graph_index_of_vertex(g, y);
    if (vx == NULL || idxy < 0) return false;

    // 头插法
    // arc_node_t* a = vx->first;
    // vx->first = (arc_node_t*) malloc(sizeof(arc_node_t));
    // vx->first->next = a;
    // vx->first->vert = idxy;

    // 尾插法, 尾插法效率低但一般和题目序号的顺序一致
    arc_node_t** a = &vx->first;
    while (*a) {
        a = &(*a)->next;
    }

    *a = (arc_node_t*) malloc(sizeof(arc_node_t));
    (*a)->next = NULL;
    (*a)->vert = idxy;

    return *a;
}

bool al_graph_remove_arc(al_graph_t* g, const basic_obj_t* x,
                         const basic_obj_t* y) {
    vert_node_t* v = al_graph_get_vertex(g, x);
    if (v == NULL) return false;
    arc_node_t* a = v->first;
    if (a && a->vert == v) {
        v->first = v->first->next;
        // 释放弧节点
        free(a);
    }
    arc_node_t* pre = a;
    a = a->next;

    // 继续查找与v邻接的边并删除
    while (a) {
        if (a->vert == v) {
            pre->next = a->next;
            free(a);
            break;
        }
        a = a->next;
    }
    return true;
}

arc_node_t* al_graph_get_arc(al_graph_t* g, const basic_obj_t* x,
                             const basic_obj_t* y) {
    vert_node_t* v = al_graph_get_vertex(g, x);
    if (v == NULL) return;
    arc_node_t* a = v->first;
    while (a) {
        if (g->vert_equals(&g->vertices[a->vert].data, y)) {
            return a;
        }
    }
    return NULL;
}

vert_node_t* al_graph_get_vertex(al_graph_t* g, const basic_obj_t* x) {
    for (int i = 0; i < g->nvert; i++) {
        if (g->vert_equals(&g->vertices[i].data, x)) {
            return &g->vertices[i];
        }
    }
    return NULL;
}

int al_graph_index_of_vertex(al_graph_t* g, const basic_obj_t* x) {
    for (int i = 0; i < g->nvert; i++) {
        if (g->vert_equals(&g->vertices[i].data, x)) {
            return i;
        }
    }
    return -1;
}

static void bfs(al_graph_t* g, int idx, vert_node_accept_fn accept, void* arg,
                sq_queue_t* q, bool* visited) {
    accept(&g->vertices[idx], arg);
    visited[idx] = true;
    *(int*) sq_queue_enq(q) = idx;
    while (!sq_queue_empty(q)) {
        // 顶点v的下标出队列
        vert_node_t* v = &g->vertices[*(int*) sq_queue_deq(q)];

        // 遍历邻接点
        arc_node_t* arc = v->first;
        while (arc) {
            if (!visited[arc->vert]) {
                // 访问
                accept(&g->vertices[arc->vert], arg);
                visited[arc->vert] = true;
                // 入队
                *(int*) sq_queue_enq(q) = arc->vert;
            }
            arc = arc->next;
        }
    }
}
void al_graph_traverse_bfs(al_graph_t* g, vert_node_accept_fn accept,
                           void* arg) {
    sq_queue_t q;
    bool* visited = (bool*) malloc(sizeof(bool) * g->nvert);
    memset(visited, 0, sizeof(bool) * g->nvert);
    sq_queue_init(&q, sizeof(int), (size_t) g->nvert);

    for (int i = 0; i < g->nvert; i++) {
        if (!visited[i]) {
            bfs(g, i, accept, arg, &q, visited);
        }
    }

    sq_queue_destory(&q);
    free(visited);
}

static void dfs(al_graph_t* g, int idx, vert_node_accept_fn accept, void* arg,
                bool* visited) {
    accept(&g->vertices[idx], arg);
    visited[idx] = true;

    arc_node_t* a = g->vertices[idx].first;
    while (a) {
        if (!visited[a->vert]) {
            dfs(g, a->vert, accept, arg, visited);
        }
        a = a->next;
    }
}
void al_graph_traverse_dfs(al_graph_t* g, vert_node_accept_fn accept,
                           void* arg) {
    bool* visited = (bool*) malloc(sizeof(bool) * g->nvert);
    memset(visited, 0, sizeof(bool) * g->nvert);

    for (int i = 0; i < g->nvert; i++) {
        if (!visited[i]) {
            dfs(g, i, accept, arg, visited);
        }
    }

    free(visited);
}
