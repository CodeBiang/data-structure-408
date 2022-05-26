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

int* swap_mn(int* arr, int m, int n) {
    // 空间复杂度 O(m + n)
    int* temp_arr = (int*) malloc((m + n) * sizeof(int));

    for (int i = 0; i < n; i++) {
        temp_arr[i] = arr[i + m];
    }

    for (int i = 0; i < m; i++) {
        temp_arr[i + n] = arr[i];
    }

    for (int i = 0; i < m + n; i++) {
        arr[i] = temp_arr[i];
    }

    free(temp_arr);
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 11, 12, 13};
    int m = 5;
    int n = 3;
    swap_mn(arr, 5, 3);
    for (size_t i = 0; i < sizeof(arr) / sizeof(int); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
