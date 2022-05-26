/**
 *
 * 6. 删除重复元素
 *
 *
 */
#include "util.h"

static void remove_all_same(sq_list_t* l) {
    int* arr = (int*) l->data;
    int ndel;

    // 遍历每个元素
    for (int i = 0; i < l->length; i++) {
        ndel = 0;
        // 从下一个开始查找相同元素
        for (int j = i + 1; j < l->length;) {
            // 元素删除后移动
            arr[j] = arr[j + ndel];
            // 相同删除
            if (arr[i] == arr[j]) {
                // 删除
                l->length--;
                ndel++;
            } else {
                j++;
            }
        }
    }
}

int main() {
    sq_list_t list;

    // sq_list_build(&list, NULL, 0);
    sq_list_build(&list, (int[]){17, 11, 9, 84, 9, 72, 9, 11, 9, 88}, 10);

    remove_all_same(&list);
    int_sq_list_print(&list);
    return 0;
}