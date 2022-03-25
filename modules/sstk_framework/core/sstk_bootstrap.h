/**
 * @file    sstk_bootstrap.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   框架启动脚手架，框架单例启动帮助接口
 */
#ifndef __SSTK_BOOTSTRAP_H_
#define __SSTK_BOOTSTRAP_H_

#include <sstk_core.h>

/* 启动引导器 */
struct sstk_bootstrap_s {
    sstk_cycle_t* cycle;

    sstk_bootstrap_t* (* const with_cycle)(sstk_bootstrap_t* const boot, sstk_cycle_t* cycle);
    sstk_bootstrap_t* (* const with_allocator)(sstk_bootstrap_t* const boot, sstk_allocator_t* allocator);
    
    sstk_bootstrap_t* (* const add_logger)(sstk_bootstrap_t* const boot, sstk_logger_t* logger);
    sstk_bootstrap_t* (* const add_chain)(sstk_bootstrap_t* const boot, sstk_chain_t* chain);

    sstk_bootstrap_t* (* const post_conf)(sstk_bootstrap_t* const boot, sstk_conf_post_t* conf);
    
    sstk_bootstrap_t* (* const init_cycle)(sstk_bootstrap_t* const boot);

    sstk_cycle_t* (* const setup)(sstk_bootstrap_t* const boot);
};

extern sstk_bootstrap_t* const sstk_bootstrap;

#endif
