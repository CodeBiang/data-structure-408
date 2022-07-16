/**
 *
 * 8.   一维数组A[m + n] 依次存放 (a1,a2,a3...am) 和 (b1, b2, b3...bn)
 *      把 数列b 放到 a前面
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

static void swap_mn(int* arr, int m, int n) {
    reverse(arr, 0, m - 1);
    reverse(arr, m, m + n - 1);
    reverse(arr, 0, m + n - 1);
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 11, 12, 13};
    int m = 5;
    int n = 3;
    swap_mn(arr, m, n);
    for (size_t i = 0; i < sizeof(arr) / sizeof(int); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
