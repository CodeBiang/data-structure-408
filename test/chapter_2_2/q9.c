/**
 *
 * 9.   有序线性表a递增存储，要求在最少时间查找x，查找后将其与后续元素替换，找不到则插入
 *
 *
 */
#include "util.h"

static void insert_or_swap_with_next(sq_list_t* l, int x) {
    int* arr = (int*) l->data;

    int left = 0;
    int right = l-> length - 1;
    int idx;

    while (left < right) {
        idx = left + (right - left) / 2;

        if (arr[idx] > x) {
            right = idx - 1;
        } else if (arr[idx] < x) {
            left = idx + 1;
        } else {
            // 与下一个替换
            if (idx != l->length - 1) {
                int temp = arr[idx];
                arr[idx] = arr[idx + 1];
                arr[idx + 1] = temp;
            }
            return;
        }
    }
    
    sq_list_insert(l, idx, &x);
}

int main() {
    sq_list_t l;
    
    sq_list_build(&l, (int[]){1, 2, 3, 4, 5, 21, 22, 23, 24, 25, 0}, 11);
    l.length--;

    insert_or_swap_with_next(&l, 21);
    int_sq_list_print(&l);

    sq_list_destory(&l);
    return 0;
}
