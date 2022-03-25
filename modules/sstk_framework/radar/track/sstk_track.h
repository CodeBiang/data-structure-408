/**
 * @file    sstk_track.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   航迹算法链
 */
#ifndef __SSTK_TRACK_H_
#define __SSTK_TRACK_H_

#include <sstk_core.h>
#include <sstk_radar.h>

typedef struct {
    uint8_t x;
} sstk_track_t;

extern sstk_chain_t sstk_track_chain;

#endif
