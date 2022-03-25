/**
 * @file example_sstk_rbtree.c
 *
 * @brief 红黑树测试
 */
#include <sstk_core.h>
#include <sstk_rbtree_if.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static struct timeval now;
static int sec, usec;
#define tick_start()         \
    sstk_gettimeofday(&now); \
    sec = now.tv_sec;        \
    usec = now.tv_usec
#define tick_stop()                 \
    sstk_gettimeofday(&now);        \
    usec = now.tv_usec - usec;      \
    sec = now.tv_sec - sec;         \
    sec = usec < 0 ? sec - 1 : sec; \
    usec = usec < 0 ? usec + 1e6 : usec;

static void rbtree_str_test() {
    sstk_rbtree_t rbt;

    sstk_str_rbtree_init(&rbt, NULL, NULL);
    sstk_rbtree_str_if_t* str_rbt = (sstk_rbtree_str_if_t*) &rbt;

    sstk_str_t s1 = sstk_string("s1");
    str_rbt->put(str_rbt, &s1, (void*) 1L);
    sstk_str_t s2 = sstk_string("s2");
    str_rbt->put(str_rbt, &s2, (void*) 2L);
    sstk_str_t s3 = sstk_string("s3");
    str_rbt->put(str_rbt, &s3, (void*) 3L);
    sstk_str_t s4 = sstk_string("s4");
    str_rbt->put(str_rbt, &s4, (void*) 4L);

    sstk_str_t s;
    s.data = (unsigned char*) "s3";
    s.len = 2;
    void* ret;
    str_rbt->get(str_rbt, &s, &ret);
    sstk_uint_t iret = (sstk_uint_t) ret;

    printf("s3 -> %d\n", (int) iret);

    sstk_rbtree_free(&rbt);
}

void main() {
    rbtree_str_test();

    sstk_rbtree_t rbt;
    sstk_int_rbtree_init(&rbt, NULL, NULL);
    sstk_rbtree_int_if_t* int_rbt = (sstk_rbtree_int_if_t*) &rbt;

    // 红黑树, 测试10W数据的读写
    tick_start();
    for (int i = 0; i < 100000; i++) {
        int_rbt->put(int_rbt, i, (void*) (sstk_uint_t) i);
    }
    tick_stop();
    sstk_log_notice(sstk_cycle->logger, "[rbtree]写入10W条数据消耗 %ds%dms%dus", sec,
             usec / 1000, usec % 1000);
    void* val;
    int_rbt->get(int_rbt, 1, &val);
    sstk_log_notice(sstk_cycle->logger, "1 -> %d", (int) (sstk_uint_t) val);

    int_rbt->put(int_rbt, 1, (void*) (sstk_uint_t) 16);
    int_rbt->get(int_rbt, 1, &val);
    sstk_log_notice(sstk_cycle->logger, "1 -> %d", (int) (sstk_uint_t) val);

    sstk_log_notice(sstk_cycle->logger, "tree size %d", (int) rbt.size);
    int_rbt->remove(int_rbt, 16);
    sstk_log_notice(sstk_cycle->logger, "rm 16, tree size %d", (int) rbt.size);


    tick_start();
    int_rbt->remove(int_rbt, 1);
    tick_stop();
    sstk_log_notice(sstk_cycle->logger, "[rbtree]删除1条数据消耗 %ds%dms%dus", sec,
             usec / 1000, usec % 1000);

    tick_start();
    for (int i = 99500; i < 99600; i++) {
        void* ret = NULL;
        int_rbt->get(int_rbt, i, &ret);
        // printf("%d\n", (int) (sstk_uint_t) ret);
    }
    tick_stop();
    sstk_log_notice(sstk_cycle->logger, "[rbtree]10W数据中查询100条数据消耗 %ds%dms%dus",
             sec, usec / 1000, usec % 1000);

    sstk_rbtree_free(&rbt);

}

