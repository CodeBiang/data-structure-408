/**
 * @file    sstk_radar_conf.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   雷达链核心模块配置定义
 * 
 * @todo    此配置需要<雷达算法人员>提出并定义各种不同信号调制方法下的参数概念，从而实现灵活拓展
 */
#ifndef __SSTK_RADAR_CONFIG_H_
#define __SSTK_RADAR_CONFIG_H_

#include <sstk_core.h>

#define SSTK_RADAR_LIGHT_SPEED 3e8

/* radar configurations 雷达配置 */
typedef struct {
    float freq;      /* carrier frequency 载波频率 */
    float bandwidth; /* sweep bandwidth 扫描带宽 */
    float prt;       /* pulse repetition interval/time 脉冲重复时间 */
    float antenna_distance; /* 天线间距 */
    sstk_uint_t fft1d;      /* 距离维, 一维FFT数量 */
    sstk_uint_t fft2d;      /* 速度维, 二维FFT数量 */
    sstk_uint_t fft3d;      /* 角度维, 三维FFT数量 */
    sstk_uint_t tx_cnt; /* 发射天线数量 / 等效虚拟发射阵列阵元数目 */
    sstk_uint_t rx_cnt; /* 接收天线数量 / 等效虚拟接收阵列阵元数目 */
} sstk_radar_conf_t;

/* radar specifications 雷达规格 */
typedef struct {
    float r_res;      /* 距离分辨率 */
    float v_res;      /* 速度分辨率 */
    float a_res;      /* 角度分辨率 */
    float wavelength; /* 波长 */
    float r_max;      /* 最远探测距离 */
    float v_max;      /* 最大速度 */
    float a_max;      /* 最大角度 */
} sstk_radar_spec_t;

typedef struct {
    sstk_radar_conf_t* conf;
    sstk_radar_spec_t* spec; /* 规格由conf生成 */
} sstk_radar_conf_ctx_t;


typedef sstk_uint_t (*sstk_radar_interpolator_fn)(void* data, size_t size,
                                                  sstk_uint_t capacity,
                                                  sstk_conf_t* conf,
                                                  void* arg);
typedef sstk_bool_t (*sstk_radar_interceptor_fn)(void* cell, sstk_uint_t index,
                                                 sstk_conf_t* conf,
                                                 void* arg);
typedef sstk_uint_t (*sstk_radar_filter_fn)(void* const in[],
                                            sstk_uint_t in_cnt, void** out,
                                            sstk_uint_t out_capacity,
                                            sstk_conf_t* conf,
                                            void* arg);

typedef struct {
    sstk_str_t name;
    sstk_radar_interpolator_fn interpolate;
    void* arg;
    sstk_queue_t queue;
} sstk_radar_interpolator_t;

typedef struct {
    sstk_str_t name;
    sstk_radar_interceptor_fn intercept;
    void* arg;
    sstk_queue_t queue;
} sstk_radar_interceptor_t;

typedef struct {
    sstk_str_t name;
    sstk_radar_filter_fn filter;
    void* arg;
    sstk_queue_t queue;
} sstk_radar_filter_t;

typedef struct sstk_radar_chain_s sstk_radar_chain_t;
typedef sstk_int_t (*sstk_radar_process_fn)(sstk_radar_chain_t* ctx, sstk_conf_t* conf);

/**
 * 雷达链处理机制
 *
 * 1. chain init_poll
 * 2. chain poll
 *          -> 插值器轮询
 *          -> 处理
 *          -> 过滤器轮询
 * 3. chain exit_poll
 */
struct sstk_radar_chain_s {
    void* data;           /* 源数据元素内存地址 */
    size_t size;          /* 数据元素大小 */
    sstk_uint_t capacity; /* 数据元素容量 */

    sstk_radar_process_fn process;

    sstk_radar_interpolator_t* interpolator; /* 插值器 数据注入 */
    sstk_queue_t interceptors;  /* 拦截器队列, 单个拦截及过滤 */
    sstk_queue_t filters; /* 过滤器队列, 两两比较的拦截过滤 */

    void** in;
    sstk_uint_t in_cnt;

    void** out; /* 输出数据会传导给下一个雷达链 */
    sstk_uint_t out_cnt;

    void** swap; /* 数据交换区 */
};


sstk_int_t sstk_radar_conf_master_handler(sstk_conf_t* conf, sstk_uint_t type,
                                          void* data);
sstk_int_t sstk_radar_chain_exit_cycle(sstk_chain_t* radar_chain);

static inline void sstk_radar_chain_ds_bind(sstk_chain_t* radar_chain,
                                            void* data, size_t size,
                                            sstk_uint_t capacity) {
    sstk_radar_chain_t* ctx = (sstk_radar_chain_t*) radar_chain->ctx;
    ctx->data = data;
    ctx->size = size;
    ctx->capacity = capacity;
    if (!ctx->out) {
        ctx->out =
            radar_chain->cycle->allocator->alloc_fn(sizeof(void*) * capacity);
        ctx->swap =
            radar_chain->cycle->allocator->alloc_fn(sizeof(void*) * capacity);
    } else {
        ctx->out = radar_chain->cycle->allocator->realloc_fn(
            ctx->out, sizeof(void*) * capacity);
        ctx->swap = radar_chain->cycle->allocator->realloc_fn(
            ctx->out, sizeof(void*) * capacity);
    }
}

/*!-- get source data --*/
#define sstk_radar_chain_data(ctx, idx) (ctx->data + ctx->size * idx)

/*!-- cast to `sstk_radar_conf_t` --*/
#define SSTK_RADAR_CONF 0x01
/*!-- cast to `sstk_radar_interpolator_t` --*/
#define SSTK_RADAR_SET_INTERPOLATOR_CONF 0x11
#define SSTK_RADAR_RM_INTERPOLATOR_CONF 0x12
/*!-- cast to `sstk_radar_filter_t` --*/
#define SSTK_RADAR_ADD_FILTER_CONF 0x21
#define SSTK_RADAR_RM_FILTER_CONF 0x22
/*!-- cast to `sstk_radar_interceptor_t` --*/
#define SSTK_RADAR_ADD_INTERCEPTOR_CONF 0x31
#define SSTK_RADAR_RM_INTERCEPTOR_CONF 0x32

#endif
