/**
 * @file al_graph.h
 * @brief Adjacency List Graph
 *
 */
#ifndef __ADJ_LIST_H__
#define __ADJ_LIST_H__

#include "basic.h"

#define AL_GRAPH_MAX_VERTICES 50

typedef struct arc_node_s arc_node_t;
typedef struct vert_node_s vert_node_t;
typedef struct al_graph_s al_graph_t;

/* arc node */
struct arc_node_s {
    int vert;          // 该弧结点指向的顶点 *下标*
    basic_obj_t data;  // 弧节点携带数据
    arc_node_t* next;
};

/* vertex node */
struct vert_node_s {
    basic_obj_t data;
    arc_node_t* first;  // 指向第一条依附于该节点的弧指针
};

/* adjacency list graph */
struct al_graph_s {
    vert_node_t vertices[AL_GRAPH_MAX_VERTICES];
    basic_obj_equals_fn vert_equals;  // 顶点节点元素的比较方法, 默认为内存比较
    int nvert;                        // number of vertices
    int narc;                         // number of arcs
};

typedef void (*arc_node_accept_fn)(arc_node_t* arc, void* arg);
typedef void (*vert_node_accept_fn)(vert_node_t* vert, void* arg);

void al_graph_init(al_graph_t* g, basic_obj_equals_fn item_eq);

bool al_graph_adjacent(const al_graph_t* g, const basic_obj_t* x,
                       const basic_obj_t* y);

/**
 * @brief 遍历某个顶点邻接的边
 *
 * @param g 图
 * @param x 顶点元素
 * @param accept 回调
 * @param arg 参数
 */
void al_graph_neighbor_arcs_foreach(al_graph_t* g, const basic_obj_t* x,
                                    arc_node_accept_fn accept, void* arg);

/**
 * @brief 遍历某个顶点邻接的顶点
 *
 * @param g 图
 * @param x 顶点元素
 * @param accept 回调
 * @param arg 参数
 */
void al_graph_neighbor_vertices_foreach(al_graph_t* g, const basic_obj_t* x,
                                        vert_node_accept_fn accept, void* arg);

/**
 * @brief 插入一个顶点
 *
 * @param g 图
 * @return vert_node_t* 顶点
 */
vert_node_t* al_graph_insert_vertex(al_graph_t* g);

/**
 * @brief 删除一个顶点
 *
 * @param g 图
 * @param x 元素值指针
 * @return true 存在并删除, false 不存在
 */
bool al_graph_delete_vertex(al_graph_t* g, const basic_obj_t* x);

/**
 * @brief 为某个顶点添加一条弧
 *
 * @param g 图
 * @param x 节点x元素值指针
 * @param y 节点y元素值指针
 * @return vert_node_t* 顶点
 */
arc_node_t* al_graph_add_arc(al_graph_t* g, const basic_obj_t* x,
                              const basic_obj_t* y);

/**
 * @brief 删除弧
 *
 * @param g 图
 * @param x 节点x元素值指针
 * @param y 节点y元素值指针
 * @return true 存在并删除, false 不存在
 */
bool al_graph_remove_arc(al_graph_t* g, const basic_obj_t* x,
                         const basic_obj_t* y);

/**
 * @brief 获取一条弧
 *
 * @param x 节点x元素值指针
 * @param y 节点y元素值指针
 * @return 弧节点
 */
arc_node_t* al_graph_get_arc( al_graph_t* g, const basic_obj_t* x,
                             const basic_obj_t* y);

/**
 * @brief 获取一个顶点
 *
 * @param x 元素值指针
 * @return 顶点节点
 */
vert_node_t* al_graph_get_vertex(al_graph_t* g, const basic_obj_t* x);

/**
 * @brief 获取一个顶点下标
 *
 * @param x 元素值指针
 * @return 顶点节点
 */
int al_graph_index_of_vertex(al_graph_t* g, const basic_obj_t* x);

/**
 * @brief 广度优先搜索
 *
 * breath-first-search
 *
 * @param g
 * @param accept
 * @param arg
 */
void al_graph_traverse_bfs(al_graph_t* g, vert_node_accept_fn accept, void* arg);

/**
 * @brief 深度优先搜索
 *
 * depth-first-search
 *
 * @param g
 * @param accept
 * @param arg
 */
void al_graph_traverse_dfs(al_graph_t* g, vert_node_accept_fn accept, void* arg);

#endif
