/**
 *
 * 12. 数列A(a0, a1, a2, ..., an-1), 其中0 =< ai < n, 若某元素存在个数 > n / 2, 称之为主元素
 * 
 *
 */
#include "util.h"

static int primary_elem_search(int* arr, int n) {
    int i, c, count = 1;
    c = arr[0];
    for (i = 1; i < n; i++) {
        if (arr[i] == c) {
            count++;
        } else {
            if (count > 0)
                count--;
            else {
                c = arr[i];
                count = 1;
            }
        }
    }
    
    if (count > 0) {
        for (i = count = 0; i < n; i++) {
            if (arr[i] == c)
                count++;
        }
    }

    return count > n / 2 ? c : -1;
}

int main() {
    int arr[] = {0, 5, 5, 3, 5, 7, 5, 5};
    // int arr[] = {3, 4, 4, 7, 7, 5, 5, 5, 7, 7};

    int primary = primary_elem_search(arr, (int) (sizeof(arr) / sizeof(int)));
    printf("%d\n", primary);
    return 0;
}
