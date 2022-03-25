/**
 * @file    sstk_cluster.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   聚类算法链
 * 
 * @fixme
 * @todo    后续聚类都需要公司内所有算法人员将类如MEANSHIFT、DBSCAN等常见算法
 *          封装在PCL(Point Cloud Libaray)库中, 实现通用的聚类，而不是专有的、
 *          封装性差地使用聚类算法
 */
#ifndef __SSTK_CLUSTER_H_
#define __SSTK_CLUSTER_H_

#include <sstk_core.h>
#include <sstk_radar.h>

#include <cfar/sstk_cfar.h>

#define SSTK_CLUSTER_CONF_PACKED
#include "sstk_cluster_conf.h"

typedef struct {
    float snr_sum;       // 关联点云的信噪比之和
    float amplitude;     // 关联点云的信号最大值
    float region_x_min;  // 关联区域的RX小值边界
    float region_x_max;  // 关联区域的RX大值边界
    float region_y_min;  // 关联区域的RY小值边界
    float region_y_max;  // 关联区域的RY大值边界
    int region_v_idx_min;  // 关联区域的多普勒索引小值边界, 作为第三个关联维度
    int region_v_idx_max;  // 关联区域的多普勒索引大值边界
    float x_th;            // 关联区域的RX关联门限
    float y_th;            // 关联区域的RY关联门限
    int v_idx_th;          // 关联区域的速度索引值关联门限
    struct {
        float l, t, r, b;
    } rect;             // 类所占实际矩形区域
    int16_t v_idx_min;  // 关联点云的最小多普勒值
    int16_t v_idx_max;  // 关联点云的最大多普勒值
    int16_t r_idx_min;  // 关联点云的最小距离维度值
    int16_t r_idx_max;  // 关联点云的最大距离维度值
    float angle_min;    // 关联点云的最小角度
    float angle_max;    // 关联点云的最大角度
} sstk_cluster_related_region_t;

typedef struct {
    uint8_t x;

    sstk_cluster_related_region_t related_region;

    sstk_cfar_t* pts;       // cfar点集合
    sstk_cfar_t* pts_tail;  // cfar点集合
    sstk_uint_t pts_cnt;    // cfar点数量
} sstk_cluster_t;

extern sstk_chain_t sstk_cluster_chain;

#endif
