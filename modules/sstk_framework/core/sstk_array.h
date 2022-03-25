/**
 * @file    sstk_array.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   自动扩容的数组数据结构
 */
#ifndef __SSTK_ARRAY_H_
#define __SSTK_ARRAY_H_

#include <sstk_core.h>

typedef struct {
    void* items;                 /* 元素数组 */
    sstk_uint_t count;           /* 元素个数 */
    size_t size;                 /* 单个元素大小 */
    sstk_uint_t capacity;        /* 元素容量 */
    sstk_allocator_t* allocator; /* 内存分配 */
} sstk_array_t;

sstk_int_t sstk_array_init(sstk_array_t* arr, sstk_allocator_t* allocator, sstk_uint_t n,
                           size_t size);
sstk_array_t* sstk_array_create(sstk_allocator_t* a, sstk_uint_t n, size_t size);
void sstk_array_destroy(sstk_array_t* arr);
void* sstk_array_push(sstk_array_t* arr);
void* sstk_array_push_n(sstk_array_t* arr, sstk_uint_t n);

#define sstk_array_at(arr, idx, type) (type*) ((char*) (arr)->items + idx * (arr)->size)

#define sstk_array_fori(itemptr, arr, i, type)                                              \
    for (itemptr = ((type*) ((arr)->items)), i = 0;                                         \
         (sstk_uint_t) itemptr < ((sstk_uint_t) (arr)->items + (arr)->size * (arr)->count); \
         itemptr = (type*) ((char*) itemptr + (arr)->size), i++)

#define sstk_array_foreach(itemptr, arr, type)                                              \
    for (itemptr = ((type*) ((arr)->items));                                                \
         (sstk_uint_t) itemptr < ((sstk_uint_t) (arr)->items + (arr)->size * (arr)->count); \
         itemptr = (type*) ((char*) itemptr + (arr)->size))

#endif
