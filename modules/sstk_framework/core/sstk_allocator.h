/**
 * @file    sstk_allocator.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   内存分配器结构
 * 
 * @todo    后续需要添加的功能包括
 *          1. 堆内存池功能, 实现框架内的内存申请、分配、扩容、内存块管理，
 *             避免每次从系统中申请内存造成性能问题
 *          2. 共享内存钩子, 为硬件平台共享内存的应用开放接口
 */
#ifndef __SSTK_ALLOCATOR_H_
#define __SSTK_ALLOCATOR_H_

#include <sstk_core.h>

struct sstk_allocator_s {
    void* (*alloc_fn)(size_t size);
    void* (*realloc_fn)(void* memory, size_t size);
    void (*dealloc_fn)(void* memory);

    /* TODO 后续使用内存池, 实现框架内的内存管理 */
    /* ... */
};

#endif
