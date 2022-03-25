/**
 * @file    sstk_kdtree.h
 * @date    2021/12/16
 * @author  赵陈淏
 * @brief   kdtree
 *          k-dimensional tree,
 * 是一种对k维空间中的实例点进行存储以便对其进行快速检索的树形数据结构。
 *          用于多维空间关键数据的搜索（如：范围搜索和最近邻搜索）。K-D树是二进制空间分割树的特殊的情况。
 *
 */
#ifndef __SSTK_KDTREE_H_
#define __SSTK_KDTREE_H_

#include <sstk_core.h>

#define SSTK_KDTREE_MAX_DIM 8

typedef struct {
    int dim;                            // 维度
    sstk_allocator_t* allocator;        // 内存分配器
    sstk_allocable_t* allocable;        // 构造析构器
} sstk_kdtree_opt_t;

struct kdtree_s;
struct kdres_s;

typedef struct kdtree_s sstk_kdtree_t;
typedef struct kdres_s sstk_kdres_t;

sstk_kdtree_t* sstk_kd_create(const sstk_kdtree_opt_t* opt);
void sstk_kd_free(sstk_kdtree_t* tree);
void sstk_kd_clear(sstk_kdtree_t* tree);

int sstk_kd_insert(sstk_kdtree_t* tree, const double* pos, void* data);
int sstk_kd_insertf(sstk_kdtree_t* tree, const float* pos, void* data);
int sstk_kd_insert3(sstk_kdtree_t* tree, double x, double y, double z, void* data);
int sstk_kd_insert3f(sstk_kdtree_t* tree, float x, float y, float z, void* data);

sstk_kdres_t* sstk_kd_nearest(sstk_kdtree_t* tree, const double* pos);
sstk_kdres_t* sstk_kd_nearestf(sstk_kdtree_t* tree, const float* pos);
sstk_kdres_t* sstk_kd_nearest3(sstk_kdtree_t* tree, double x, double y, double z);
sstk_kdres_t* sstk_kd_nearest3f(sstk_kdtree_t* tree, float x, float y, float z);

sstk_kdres_t* sstk_kd_nearest_range(sstk_kdtree_t* tree, const double* pos, double range);
sstk_kdres_t* sstk_kd_nearest_rangef(sstk_kdtree_t* tree, const float* pos, float range);
sstk_kdres_t* sstk_kd_nearest_range3(sstk_kdtree_t* tree, double x, double y, double z, double range);
sstk_kdres_t* sstk_kd_nearest_range3f(sstk_kdtree_t* tree, float x, float y, float z, float range);

sstk_kdres_t* sstk_kd_nearest_range_ordered(sstk_kdtree_t* tree, const double* pos, double range);
sstk_kdres_t* sstk_kd_nearest_rangef_ordered(sstk_kdtree_t* tree, const float* pos, float range);
sstk_kdres_t* sstk_kd_nearest_range3_ordered(sstk_kdtree_t* tree, double x, double y, double z, double range);
sstk_kdres_t* sstk_kd_nearest_range3f_ordered(sstk_kdtree_t* tree, float x, float y, float z, float range);

void sstk_kd_res_free(sstk_kdres_t* set);
int sstk_kd_res_size(sstk_kdres_t* set);
void sstk_kd_res_rewind(sstk_kdres_t* set);
sstk_bool_t sstk_kd_res_end(sstk_kdres_t* set);
sstk_bool_t sstk_kd_res_has_next(struct kdres_s* rset);
sstk_bool_t sstk_kd_res_next(sstk_kdres_t* set);
void* sstk_kd_res_item_data(sstk_kdres_t* set);

#endif
