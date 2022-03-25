/**
 * @file    sstk_radar.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   雷达链核心模块
 */
#ifndef __SSTK_RADAR_H_
#define __SSTK_RADAR_H_

#include <sstk_core.h>

#include <sstk_radar_config.h>


sstk_int_t sstk_radar_chain_init_chain(sstk_chain_t* chain);
sstk_int_t sstk_radar_chain_init_poll(sstk_chain_t* chain);
sstk_int_t sstk_radar_chain_poll(sstk_chain_t* chain);

extern sstk_chain_t sstk_radar_chain;

#define SSTK_RADAR_CHAIN 0x00000002

#endif
