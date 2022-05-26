/**
 *
 * 10.  数组(长度n) 左移 p (> 0, < n) 个item
 *
 *      如   abcdefg 左移4
 *      变为 efgabcd
 *
 * (1) 思路通过三次翻转实现
 * (3) 复杂度
 *     空间 O(1)
 *     时间 O(n)
 * 
 *
 */
#include "util.h"

static void reverse(int* arr, int begin, int end) {
    int temp;
    while (begin < end) {
        temp = arr[begin];
        arr[begin] = arr[end];
        arr[end] = temp;

        begin++;
        end--;
    }
}

static void left_move(int* arr, int n, int p) {
    reverse(arr, 0, p - 1);
    reverse(arr, p, n - 1);
    reverse(arr, 0, n -1);
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    int len = (int) (sizeof(arr) / sizeof(int));

    left_move(arr, len, 5);

    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
