#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitree.h"
#include "linked_queue.h"

void pre2post_impl(int pre[], int post[], int pre_start, int pre_end,
                   int post_start, int post_end) {
    if (pre_end < pre_start) return;

    // 后序最后一个 为 先序 第一个
    printf("idx %d -> %d %d\n", post_end, pre[pre_start], pre_start);
    post[post_end] = pre[pre_start];
    // 取中间位置分割左右子树元素
    int half = (pre_end - pre_start) / 2;
    // 处理左右子树
    pre2post_impl(pre, post, pre_start + 1, pre_start + half, post_start,
                  post_start + half - 1);

    pre2post_impl(pre, post, pre_start + half + 1, pre_end, post_start + half,
                  post_end - 1);
}

void pre2post(int pre[], int post[], int size) {
    pre2post_impl(pre, post, 0, size - 1, 0, size - 1);
}

int main() {
    //        1
    //    2       3
    //  4   5   6   7
    // 8 9 A B C D E F
    // 1 2 4 8  9  5 10 11 3  6  12 13 7  14 15
    // 8 9 4 10 11 5 2  12 13 6  14 15 7  3  1
    int pre[] = {1, 2, 4, 8, 9, 5, 10, 11, 3, 6, 12, 13, 7, 14, 15};
    int post[15];
    pre2post(pre, post, 15);
    for (int i = 0; i < 15; i++) {
        printf("%d ", post[i]);
    }
    printf("\n");

    return 0;
}
