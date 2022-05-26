/**
 *
 * 2.   设计一个高效算法逆置顺序表，空间复杂度为O1
 *
 *
 */
#include "util.h"

static void reverse(sq_list_t* l) {
    int temp;

    int len = l->length;
    int half = len / 2;

    for (int i = 0; i < half; i++) {
        temp = sq_list_at(l, i, int);
        sq_list_at(l, i, int) = sq_list_at(l, len - i - 1, int);
        sq_list_at(l, len - i - 1, int) = temp;
    }
}

int main() {
    sq_list_t list;

    // sq_list_build(&list, NULL, 0);
    sq_list_build(&list, (int[]){17, 11, 9, 84, 83, 72, 24, 12, 77, 88}, 10);

    reverse(&list);
    int_sq_list_print(&list);
    return 0;
}