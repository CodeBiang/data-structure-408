/**
 *
 * 11. 两个等长升序序列A,B的中位数
 * 
 *      A [2, 4, 6, 8, 20]
 *      B [11, 12, 13, 15, 17]
 * 
 *      A ∪ B [2, 4, 6, 8, 11, 12, 13, 15, 17, 20]
 * 
 * (1) 因为等长且有序, 所以两个序列满足以下特点：
 *      1. A，B 分别取得 中位数a, b
 *          a == b 则 中位数为a 返回
 *          a > b 则取 a左侧数列 和 b 右侧数列 组成的数列中位数仍为 A,B 的中位数
 *          a < b 反之
 *      
 *
 */
#include "util.h"

static int mid_search(int* arr_a, int* arr_b, int l) {
    int idx = (l - 1) / 2;
    int a = arr_a[idx];
    int b = arr_b[idx];
    
    if (l == 0) {
        // a == b ==> a || b
        return a > b ? b : a;
    }

    if (a == b) 
        return a;
    else if (a > b) 
        return mid_search(&arr_a[0], &arr_b[idx + 1], idx);
    else 
        return mid_search(&arr_a[idx + 1], &arr_b[0], idx);
}

int main() {
    int arr_a[] = {11, 13, 15, 17, 19};
    int arr_b[] = {2, 4, 6, 8, 20};

    int mid = mid_search(arr_a, arr_b, 5);
    printf("%d\n", mid);
    return 0;
}
