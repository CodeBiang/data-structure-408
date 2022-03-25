/**
 * @file example_sstk_heap5.c
 *
 * @brief 静态堆内存分配测试
 */
#include <sstk_core.h>
#include <sstk_heap5.h>

#include <assert.h>

static char heap_mem1[4096];
static char heap_mem2[2048];
static sstk_heap_region_t regions[] = {
    {
        .addr = heap_mem1, 
        .size = sizeof(heap_mem1)
    },  
    {
        .addr = heap_mem2, 
        .size = sizeof(heap_mem2)
    },
    { NULL, 0 } 
};

int main() {
    struct sstk_heap_stats_s stats;

    sstk_heap_init(regions);

    sstk_heap_get_stats(&stats);
    printf("初始剩余堆内存 %ld\n", stats.bytes_of_avalable_space);

    int* pint = sstk_heap_malloc(sizeof(int) * 100);

    sstk_heap_get_stats(&stats);
    printf("分配后剩余堆内存 %ld\n", stats.bytes_of_avalable_space);

    sstk_heap_free(pint);

    sstk_heap_get_stats(&stats);
    printf("释放后剩余堆内存 %ld\n", stats.bytes_of_avalable_space);

    return 0;
}
