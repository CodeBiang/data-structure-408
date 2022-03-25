/**
 * @file    
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   
 */
#ifndef __SSTK_CFAR_H_
#define __SSTK_CFAR_H_

#include <sstk_core.h>
#include <sstk_radar.h>

#define SSTK_CFAR_CONF_PACKED
#include "sstk_cfar_conf.h"

typedef struct sstk_cfar_s sstk_cfar_t;

struct sstk_cfar_s {
    uint16_t r_idx;   // 距离索引值
    int16_t v_idx;    // 速度/多普勒索引值
    int16_t a_idx;    // 角度索引值
    float snr;        // 信噪比
    float amplitude;  // 幅度
    float noise;      // 底噪
    float range;      // 径向距离
    float speed;      // 径向速度
    float azimuth;    // 方位角
    float x;          // 笛卡尔坐标系x
    float y;          // 笛卡尔坐标系y
    float z;

    struct sstk_cfar_s* next;
};

extern sstk_chain_t sstk_cfar_chain;

#endif
