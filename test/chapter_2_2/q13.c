/**
 *
 * 13. 找数列中的最小未出现的正整数，要求时间高效
 * 
 *
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static int find_min_positive(int* arr, int n) {
    // 因为长度是n, 所以最小未出现的正整数 x ∈ (0, n)
    bool* bitmap = (bool*) malloc(sizeof(bool) * n);

    memset(bitmap, 0, sizeof(bool) * n);

    for (int i = 0; i < n; i++) {
        if (arr[i] > 0 && arr[i] < n) {
            bitmap[arr[i] - 1] = true;
        }
    }

    for (int i = 0; i < n; i++) {
        if (!bitmap[i]) {
            free(bitmap);
            return i + 1;
        }
    }

    // never
    free(bitmap);
    return -1;
}

int main() {
    int arr[] = {0, 1, 2, 15, 18, 79, 6, 4, 4, 5};

    int min_positive = find_min_positive(arr, (int) (sizeof(arr) / sizeof(int)));
    printf("%d\n", min_positive);
    return 0;
}
