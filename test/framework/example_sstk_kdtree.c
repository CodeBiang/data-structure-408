/**
 * @file example_sstk_kdtree.c
 *
 * @brief kdtree 示例
 */
#include <sstk_core.h>
#include <sstk_kdtree.h>

int main() {
    sstk_kdtree_opt_t opt = {
        .allocable = &sstk_default_allocable,
        .allocator = sstk_default_allocator,
        .dim = 3
    };
    
    sstk_kdtree_t* kdtree = sstk_kd_create(&opt);

    sstk_kd_insert3(kdtree, 0, 0, 0, "no.0");
    sstk_kd_insert3(kdtree, 10, 11, 12, "no.1");
    sstk_kd_insert3(kdtree, 18, 14, 16, "no.2");
    sstk_kd_insert3(kdtree, 4, 4, 4, "no.4");
    sstk_kd_insert3(kdtree, 3, 5, 4, "no.5");
    sstk_kd_insert3(kdtree, 2, 6, 2, "no.6");
    sstk_kd_insert3(kdtree, 1, 7, 4, "no.7");

    // 查询(0, 0, 0)点, 范围为7以内的所有数据
    // ordered 按照 由近到远排序, 无ordered无排序
    sstk_kdres_t* ret = sstk_kd_nearest_range3_ordered(kdtree, 0, 0, 0, 7);

    // 遍历结果
    for (; sstk_kd_res_has_next(ret); sstk_kd_res_next(ret)) {
        printf("cur %s\n", (char*) sstk_kd_res_item_data(ret));
    }

    // 释放结果集
    sstk_kd_res_free(ret);

    return 0;
}