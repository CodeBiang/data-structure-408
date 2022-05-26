/**
 *
 * 3.   长度为n顺序表L，编写一个时间复杂度On、空间O1的算法，该算法删除线性表中所有值为n的元素
 *
 *
 */
#include "util.h"

static void remove_all(sq_list_t* l, int item) {
    if (l->length == 0) return;

    int ndel = 0;
    int* arr = (int*) l->data;

    for (int i = 0; i < l->length;) {
        arr[i] = arr[i + ndel];
        if (arr[i] != item) {
            i++;
        } else {
            l->length--;
            ndel++;
        }
    }
}

int main() {
    sq_list_t list;

    // sq_list_build(&list, NULL, 0);
    sq_list_build(&list, (int[]){17, 11, 9, 84, 9, 72, 9, 12, 9, 88}, 10);

    remove_all(&list, 9);
    int_sq_list_print(&list);
    return 0;
}