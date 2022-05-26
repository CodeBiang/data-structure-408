/**
 *
 * 1.   从顺序表中删除具有最小值的元素（假设唯一）并由函数返回被删除元素的值。
 *      空出的位置由最后一个元素补充，若顺序表为空，则显示出错信息并推出运行。
 *
 *
 */
#include <limits.h>

#include "util.h"

static int remove_min(sq_list_t* list) {
    int last_item;
    int cur;
    int min = INT_MAX;
    int min_idx = -1;

    if (list->length == 0) {
        printf("empty list\n");
        return -1;
    }

    last_item = sq_list_at(list, list->length - 1, int);

    for (int i = 0; i < list->length; i++) {
        cur = sq_list_at(list, i, int);

        if (cur < min) {
            min = cur;
            min_idx = i;
        }
    }

    sq_list_at(list, min_idx, int) = last_item;

    return min;
}

int main() {
    sq_list_t list;

    // sq_list_build(&list, NULL, 0);
    sq_list_build(&list, (int[]){17, 11, 9, 84, 83, 72, 24, 12, 77, 88}, 10);

    remove_min(&list);
    int_sq_list_print(&list);
    return 0;
}