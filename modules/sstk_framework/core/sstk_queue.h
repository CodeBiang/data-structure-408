/**
 * @file    sstk_queue.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   双向链表/队列
 */
#ifndef __SSTK_QUEUE_H_
#define __SSTK_QUEUE_H_

#include <sstk_core.h>

typedef struct sstk_queue_s sstk_queue_t;

struct sstk_queue_s {
    sstk_queue_t* prev;
    sstk_queue_t* next;
};

#define sstk_queue_initializer(q) \
    { (q), (q) }

/* 队列容器初始化 */
#define sstk_queue_init(q) \
    (q)->prev = q;         \
    (q)->next = q

/* 队列容器空 */
#define sstk_queue_empty(h) (h == (h)->prev)

/* 队列头插 */
#define sstk_queue_insert_head(h, x) \
    (x)->next = (h)->next;           \
    (x)->next->prev = x;             \
    (x)->prev = h;                   \
    (h)->next = x

/* 队列头插 */
#define sstk_queue_insert_after sstk_queue_insert_head

/* 队列尾插 */
#define sstk_queue_insert_tail(h, x) \
    (x)->prev = (h)->prev;           \
    (x)->prev->next = x;             \
    (x)->next = h;                   \
    (h)->prev = x

/* 队列容器头 */
#define sstk_queue_head(h) (h)->next

/* 队列容器尾 */
#define sstk_queue_last(h) (h)->prev

#define sstk_queue_sentinel(h) (h)

/* 队列下一个节点 */
#define sstk_queue_next(q) (q)->next

/* 队列上一个节点 */
#define sstk_queue_prev(q) (q)->prev

/* 队列元素删除 */
#define sstk_queue_remove(x)     \
    (x)->next->prev = (x)->prev; \
    (x)->prev->next = (x)->next

/* 队列分割 */
#define sstk_queue_split(h, q, n) \
    (n)->prev = (h)->prev;        \
    (n)->prev->next = n;          \
    (n)->next = q;                \
    (h)->prev = (q)->prev;        \
    (h)->prev->next = h;          \
    (q)->prev = n;

/* 队列拼接 */
#define sstk_queue_add(h, n)     \
    (h)->prev->next = (n)->next; \
    (n)->next->prev = (h)->prev; \
    (h)->prev = (n)->prev;       \
    (h)->prev->next = h;

#define sstk_queue_data(q, type, link) \
    (type *)((unsigned char *)q - offsetof(type, link))

#define sstk_queue_foreach(q, container)                                      \
    for (q = sstk_queue_head(container); q != sstk_queue_sentinel(container); \
         q = sstk_queue_next(q))

#define sstk_queue_rforeach(q, container)                                     \
    for (q = sstk_queue_last(container); q != sstk_queue_sentinel(container); \
         q = sstk_queue_prev(q))

void sstk_queue_sort(sstk_queue_t *queue,
                     sstk_int_t (*cmp)(const sstk_queue_t *,
                                       const sstk_queue_t *));

#endif
