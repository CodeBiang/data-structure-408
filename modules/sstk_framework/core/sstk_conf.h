/**
 * @file    
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   配置结构与接口
 *          配置结构定义了
 *          1. 周期调度外部向核心cycle中提交配置的配置解析、控制接口
 *          2. 配置提交(post)结构，通过此结构向链模块提交配置
 */
#ifndef __SSTK_CONF_H_
#define __SSTK_CONF_H_

#include <sstk_core.h>

typedef sstk_int_t (*sstk_conf_handler_fn)(sstk_conf_t* conf, sstk_uint_t type, void* data);

struct sstk_conf_s {
    void* ctx;
    
    sstk_conf_handler_fn master_handler;
    sstk_conf_handler_fn slave_handler;

    sstk_chain_t* chain;
};

typedef struct {
    sstk_str_t name; /* 配置的目标链 */

    sstk_uint_t conf_type;
    void* data;

    sstk_queue_t queue;
} sstk_conf_post_t;

void sstk_conf_post(sstk_cycle_t* cycle, sstk_conf_post_t* post);

#endif
