/**
 *
 * 14. 三元组最小距离问题
 *
 *
 */
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _arrlen(arr) ((int) (sizeof(arr) / sizeof(int)))

static inline void max_min(int a, int b, int c, int* min, int* max) {
    if (a < b) {
        *min = a < c ? a : c;
        *max = b > c ? b : c;
    } else {
        *min = b < c ? b : c;
        *max = a > c ? a : c;
    }
}

int main() {
    int s1[] = {-1, 0, 9};
    int s2[] = {-25, -10, 10, 11};
    int s3[] = {2, 9, 17, 30, 41};

    int len1 = _arrlen(s1);
    int len2 = _arrlen(s2);
    int len3 = _arrlen(s3);

    int i, j, k, max, min;
    int shortest_dist = INT_MAX;
    int dist;

    // int i1, j1, k1;
    // i1 = j1 = k1 = 0;

    i = j = k = 0;

    while (i < len1 && j < len2 && k < len3) {
        // 获取最大最小值
        max_min(s1[i], s2[j], s3[k], &min, &max);

        dist = (max - min) * 2;

        if (dist < shortest_dist) {
            shortest_dist = dist;
            // i1 = i; j1 = j; k1 = k;
        }

        if (s1[i] == min)
            i++;
        else if (s2[j] == min)
            j++;
        else
            k++;
    }

    // printf("%d, %d, %d -> %d\n", s1[i1], s2[j1], s3[k1], shortest_dist);
    printf("%d\n", shortest_dist);

    return 0;
}
