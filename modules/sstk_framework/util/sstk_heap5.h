/**
 * @file    sstk_heap5.h
 * @date    2021/11/10
 * @author  赵陈淏
 * @brief   freertos中实现heap_5堆管理程序的改造与封装
 */
#ifndef __SSTK_HEAP_5_H_
#define __SSTK_HEAP_5_H_

#include <stddef.h>
#include <stdint.h>

#define mtCOVERAGE_TEST_MARKER()
/* 字节对齐配置 */
#define portBYTE_ALIGNMENT 8
#define portMAX_DELAY 65535

#if portBYTE_ALIGNMENT == 32
#define portBYTE_ALIGNMENT_MASK (0x001f)
#endif

#if portBYTE_ALIGNMENT == 16
#define portBYTE_ALIGNMENT_MASK (0x000f)
#endif

#if portBYTE_ALIGNMENT == 8
#define portBYTE_ALIGNMENT_MASK (0x0007)
#endif

#if portBYTE_ALIGNMENT == 4
#define portBYTE_ALIGNMENT_MASK (0x0003)
#endif

#if portBYTE_ALIGNMENT == 2
#define portBYTE_ALIGNMENT_MASK (0x0001)
#endif

#if portBYTE_ALIGNMENT == 1
#define portBYTE_ALIGNMENT_MASK (0x0000)
#endif

typedef struct sstk_heap_region_s {
    uint8_t* addr;
    size_t size;
} sstk_heap_region_t;

/* Used to pass information about the heap out of sstk_heap_get_stats(). */
typedef struct sstk_heap_stats_s {
    /* The total heap size currently available - this is the sum of all the free blocks, not the
     * largest block that can be allocated. */
    size_t bytes_of_avalable_space;
    /* The maximum size, in bytes, of all the free blocks within the heap at the time
     * sstk_heap_get_stats() is called. */
    size_t bytes_of_largest_free_block;
    /* The minimum size, in bytes, of all the free blocks within the heap at the time
     * sstk_heap_get_stats() is called. */
    size_t bytes_of_smallest_free_block;
    /* The number of free memory blocks within the heap at the time sstk_heap_get_stats() is called.
     */
    size_t n_free_blocks;
    /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap
     * since the system booted. */
    size_t min_ever_free_bytes_remaining;
    /* The number of calls to sstk_heap_malloc() that have returned a valid memory block. */
    size_t n_successful_allocations;
    /* The number of calls to sstk_heap_free() that has successfully freed a block of memory. */
    size_t n_successful_frees;
} sstk_heap_stats_t;

void sstk_heap_init(const sstk_heap_region_t* const regions);
void* sstk_heap_malloc(size_t size);
void sstk_heap_free(void* p);
void sstk_heap_get_stats(sstk_heap_stats_t* heap_stats);

#endif

