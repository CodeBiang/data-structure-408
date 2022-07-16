#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define get_parent_index(i) (i - 1) / 2

int get_common_parent_value(int tree[], int i, int j) {
    if (i == j) {
        return tree[i];
    } else if (i < j) {
        return get_common_parent_value(tree, i, get_parent_index(j));
    } else {
        return get_common_parent_value(tree, get_parent_index(i), j);
    }
}

int main() {
    int tree[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t n = sizeof(tree) / sizeof(int);
    size_t h = (int) log2(n) + 1;
    // 最后一行的个数量
    size_t max_n = pow(2, h - 1);
    // 偶数个
    bool odd = max_n % 2 == 0;
    // 占位符宽度
    size_t w = max_n * 2 - 1;

    printf("个数为 %d\n高度为%d\n%d\n", n, h, w);

    int idx = 0;
    for (size_t i = 0, level_n = 1; i < h; i++, idx += level_n, level_n *= 2) {
        // 间隔
        int sep = (w - level_n) / (level_n + 1) + 1;
        for (size_t j = 0; j < level_n && idx + j < n; j++) {
            if (sep == 0) {
                printf(j == 0 ? "" : " ");
            }
            for (size_t k = 0; k < sep; k++) {
                printf(" ");
            }
            printf("%d", tree[idx + j]);
        }
        printf("\n");
    }

    int i = 10, j = 9;
    printf("%d, %d 公共父节点为 %d\n", i, j, get_common_parent_value(tree, i, j));

    return 0;
}
