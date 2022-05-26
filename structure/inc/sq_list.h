#ifndef __SQ_LIST_LIST_H__
#define __SQ_LIST_LIST_H__

#include <stddef.h>
#include <stdbool.h>

/* 顺序表 */
typedef struct sq_list_s {
    void* data;
    int length;
    size_t item_size;
    size_t capacity;
} sq_list_t;

/**
 * @brief 初始化顺序表
 * 
 * @param l 顺序表
 * @param elem_size 元素大小
 * @param capacity 容量
 */
bool sq_list_init(sq_list_t* l, size_t elem_size, size_t capacity);

void sq_list_destory(sq_list_t* l);

void* sq_list_push(sq_list_t* l);

bool sq_list_insert(sq_list_t* l, int idx, const void* elem);

bool sq_list_delete(sq_list_t* l, int idx, void* elem);

int sq_list_index_of(sq_list_t* l, const void* elem, bool (*equals)(const void*, const void*));

#define sq_list_empty(l) ((l)->length == 0)

#define sq_list_at(l, idx, item_type) (((item_type*) (l)->data)[idx])

#endif
