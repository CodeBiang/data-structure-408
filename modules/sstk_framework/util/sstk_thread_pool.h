/**
 * @file    sstk_thread_pool.h
 * @date    2021/11/18
 * @author  赵陈淏
 * @brief   线程池
 * 
 */
#ifndef __SSTK_THREAD_POOL_H_
#define __SSTK_THREAD_POOL_H_

#include <sstk_core.h>

#if (SSTK_THREADS)

typedef struct sstk_thread_pool_s sstk_thread_pool_t;
typedef struct sstk_future_s sstk_future_t;
typedef void (*sstk_future_listener_fn)(void* result);

struct sstk_future_s {
    sstk_bool_t (*wait)(struct sstk_future_s* future, void** result);
    sstk_bool_t (*timed_wait)(struct sstk_future_s* future, struct timespec* timeout, void** result);
    struct sstk_future_s* (*add_listener)(struct sstk_future_s* future, sstk_future_listener_fn listener);
    sstk_bool_t (*ready)(struct sstk_future_s* future);
};

sstk_thread_pool_t* sstk_thread_pool_create(sstk_logger_t* logger, int max_thread_num, int queue_capacity);

/**
 * 添加异步工作任务
 * 
 * @param pool 线程池指针
 * @param worker 异步执行程序
 * @return 0成功
 */
int sstk_thread_pool_submit(sstk_thread_pool_t* pool, void* (*run)(void* arg), void* arg, sstk_future_t* future);

/**
 * 销毁
 * 
 * @param pool 线程池指针
 * @return 0成功
 */
int sstk_thread_pool_destroy(sstk_thread_pool_t* pool);

/**
 * 销毁并释放
 * 
 * @param ppool 线程池指针的指针
 * @return 0成功
 */
int sstk_thread_pool_free(sstk_thread_pool_t** ppool);

/**
 * 创建future
 * 
 * @return future
 */
sstk_future_t* sstk_future_create();

/**
 * 释放future
 * 
 * @param future f
 */
void sstk_future_free(sstk_future_t* future);

/**
 * 等待任务执行完成
 * 
 * void* ret;
 * sstk_future_wait(future, &ret);
 * ... handle ret
 * 
 * ret由任务返回, 其值自行处理(比如需要释放)
 * 
 * @param future f
 * @param result 结果指针的引用指针
 */
sstk_bool_t sstk_future_wait(struct sstk_future_s* future, void** result);

/**
 * timedwait 定时等待
 * 
 * @param future f
 * @param timeout 超时时间(绝对时间)
 * @param result 结果指针的引用指针
 */
sstk_bool_t sstk_future_timed_wait(struct sstk_future_s* future, struct timespec* timeout, void** result);

/**
 * 添加监听器
 * 
 * @param future f
 * @param listener 回调
 */
struct sstk_future_s* sstk_future_add_listener(struct sstk_future_s* future, sstk_future_listener_fn listener);


/**
 * 是否执行完成
 * 
 * @param future f
 * @return true 执行完成
 */
sstk_bool_t sstk_future_ready(struct sstk_future_s* future);

#endif

#endif
