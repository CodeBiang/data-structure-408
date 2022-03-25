/**
 * @file    sstk_chain.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   链模块, 框架调度的核心数据结构
 */
#ifndef __SSTK_CHAIN_H_
#define __SSTK_CHAIN_H_

#include <sstk_core.h>

typedef sstk_int_t (*sstk_chain_handler_fn)(sstk_chain_t* chain);

/* 流程链结构 */
struct sstk_chain_s {
    sstk_str_t name;     /* 链名称 */
    sstk_uint_t version; /* 链版本 */
    sstk_uint_t type;    /* 链类型 */

    void* ctx;         /* 上下文 xxx_chain_t */
    sstk_conf_t* conf; /* 配置 */
    sstk_int_t expire; /* 剩余周期数 */

    sstk_chain_handler_fn init_poll; /* 处理前 */
    sstk_chain_handler_fn poll;      /* 处理器 */
    sstk_chain_handler_fn exit_poll; /* 处理后 */

    sstk_chain_handler_fn init_chain; /* 初始化链关系后回调, 此时链关系已经建立完成, 但动态添加的链条不会执行 */
    sstk_chain_handler_fn init_cycle; /* cycle 初始化调度回调, 此时链关系可能未建立完成 */
    sstk_chain_handler_fn exit_cycle; /* cycle 终止调度回调 */

    sstk_cycle_t* cycle;  /* 周期调度器 */
    sstk_queue_t queue;   /* 双向链 */
    unsigned suspend : 1; /* 挂起 */
};

typedef struct {
    sstk_str_t name;

    sstk_int_t (*init_conf)(sstk_cycle_t* cycle, sstk_conf_t* conf);
} sstk_core_chain_t;

sstk_int_t sstk_init_chains(sstk_cycle_t* cycle);
sstk_int_t sstk_add_chain(sstk_cycle_t* cycle, sstk_chain_t* chain);
sstk_chain_t* sstk_remove_chain_by_name(sstk_cycle_t* cycle, sstk_str_t name);
sstk_int_t sstk_remove_chain(sstk_cycle_t* cycle, sstk_chain_t* chain);

#define sstk_chain_queue_data(q) sstk_queue_data(q, sstk_chain_t, queue)

#define sstk_chain_prev(chain)                                   \
    (sstk_queue_prev(&chain->queue) != &chain->cycle->queue      \
         ? sstk_chain_queue_data(sstk_queue_prev(&chain->queue)) \
         : NULL)

#define sstk_chain_next(chain)                                   \
    (sstk_queue_next(&chain->queue) != &chain->cycle->queue      \
         ? sstk_chain_queue_data(sstk_queue_next(&chain->queue)) \
         : NULL)

#define __sstk_chain_declare(_chain_name)                \
    sstk_chain_t _chain_name##_chain = {                         \
    .name = sstk_string(sstk_cstringify(_chain_name)),   \
    .expire = -1,

#define __chain_name(_name) .name = sstk_string(_name),
#define __chain_version(_version) .version = _version,
#define __chain_type(_type) .type = _type,
#define __chain_ctx(_ctx) .ctx = _ctx,
#define __chain_conf(_conf) .conf = _conf,
#define __chain_expire(_expire) .expire = _expire,
#define __on_init_poll(_init_poll) .init_poll = _init_poll,
#define __on_poll(_poll) .poll = _poll,
#define __on_exit_poll(_exit_poll) .exit_poll = _exit_poll,
#define __on_init_chain(_init_chain) .init_chain = _init_chain,
#define __on_init_cycle(_init_cycle) .init_cycle = _init_cycle,
#define __on_exit_cycle(_exit_cycle) .exit_cycle = _exit_cycle,

/* C99以下或者某些特殊编译器不支持__VA_ARGS__ */
#define __sstk_chain_declare_end(_chain_name)                          \
    .cycle = NULL, .queue = sstk_queue_initializer(&(_chain_name##_chain).queue) \
    };

#endif
