/**
 * @file    sstk_cycle.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   核心调度器
 */
#ifndef __SSTK_CYCLE_H_
#define __SSTK_CYCLE_H_

#include <sstk_core.h>

struct sstk_cycle_s {
    sstk_logger_t* logger; /* 日志输出器 */
    sstk_allocator_t* allocator; /* 内存分配器 */

    void (*exception_handler)(sstk_chain_t* chain,
                              sstk_int_t code); /* 异常处理器 */

    sstk_queue_t queue;           /* 流程链节点 */
    sstk_queue_t conf_post_queue; /* 配置队列 */

    sstk_hash_map_t* chain_map;
};

/* 全局cycle */
extern volatile sstk_cycle_t* sstk_cycle;

/* 全局默认分配器 */
extern sstk_allocator_t* sstk_default_allocator;

/**
 * 周期调度句柄初始化
 *
 * @param cycle 句柄
 */
sstk_int_t sstk_init_cycle(sstk_cycle_t* cycle);

/**
 * 周期调度退出
 *
 * @param cycle 句柄
 */
sstk_int_t sstk_exit_cycle();

/**
 * 周期调度装载
 *
 * @param cycle 句柄
 */
sstk_int_t sstk_setup_cycle(sstk_cycle_t* cycle);

/**
 * 链轮询
 *
 * @param cycle 句柄
 */
void sstk_cycle_poll(sstk_cycle_t* cycle);

#endif
