/**
 * @file    sstk_core.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   数据处理框架核心
 */
#ifndef __SSTK_CORE_H_
#define __SSTK_CORE_H_

#include <sstk_config.h>

#ifdef __GNUC__
#define __sstk_unused __attribute__((unused))
#else
#define __sstk_unused
#endif

#define SSTK_OK 0
#define SSTK_ERR -1
#define SSTK_ABORT -2
#define SSTK_OVERSIZE -3

typedef struct sstk_conf_s sstk_conf_t;
typedef struct sstk_cycle_s sstk_cycle_t;
typedef struct sstk_chain_s sstk_chain_t;
typedef struct sstk_logger_s sstk_logger_t;
typedef struct sstk_allocator_s sstk_allocator_t;

typedef struct sstk_bootstrap_s sstk_bootstrap_t;

#include <sstk_atomic.h>
#include <sstk_queue.h>
#include <sstk_allocator.h>
#include <sstk_time.h>
#include <sstk_string.h>
#include <sstk_array.h>

#include <sstk_conf.h>
#include <sstk_log.h>
#include <sstk_hash.h>
#include <sstk_hash_map.h>
#include <sstk_rbtree.h>
#include <sstk_hash2.h>
#include <sstk_cycle.h>
#include <sstk_chain.h>

#include <sstk_bootstrap.h>

#define sstk_abs(value) (((value) >= 0) ? (value) : -(value))
#define sstk_max(val1, val2) ((val1 < val2) ? (val2) : (val1))
#define sstk_min(val1, val2) ((val1 > val2) ? (val2) : (val1))
#define sstk_req_nonnull(val) if (val) val
#define sstk_cstringify_pre(str) #str
#define sstk_cstringify(str) sstk_cstringify_pre(str)

#define SSTK_CORE_CHAIN 0x00000001

#endif
