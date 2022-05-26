/**
 *
 * 4.   删除所有s和t之间的元素，有序
 * 5.   删除所有s和t之间的元素，无序
 *
 */
#include "util.h"

static void remove_all_in_range5(sq_list_t* l, int s, int t) {
    if (l->length == 0) {
        printf("empty list\n");
        return;
    }

    if (s >= t) {
        printf("invalid range\n");
        return;
    }

    int left_idx = -1, right_idx = -1;

    int* arr = (int*) l->data;

    for (int i = 0; i < l->length; i++) {
        if (arr[i] > s) {
            left_idx = i;
            break;
        }
    }

    for (int i = 0; i < l->length; i++) {
        if (arr[l->length - i - 1] < t) {
            right_idx = l->length - i - 1;
            break;
        }
    }
    
    int range = right_idx + 1 - left_idx;
    printf("%d\n", right_idx);
    for (int i = left_idx; i < right_idx; i++) {
        arr[i] = arr[i + range];
    }

    l->length -= range;
}

static void remove_all_in_range6(sq_list_t* l, int s, int t) {
    if (l->length == 0) {
        printf("empty list\n");
        return;
    }

    if (s >= t) {
        printf("invalid range\n");
        return;
    }

    int ndel = 0;
    int* arr = (int*) l->data;

    for (int i = 0; i < l->length;) {
        arr[i] = arr[i + ndel];
        if (arr[i] <= s || arr[i] >= t) {
            i++;
        } else {
            l->length--;
            ndel++;
        }
    }
}

static void q5() {
    sq_list_t list;

    sq_list_build(&list, (int[]){1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13}, 12);

    remove_all_in_range5(&list, 8, 12);
    int_sq_list_print(&list);
}

static void q6() {
    sq_list_t list;

    sq_list_build(&list, (int[]){17, 11, 9, 84, 9, 72, 9, 11, 9, 88}, 10);

    remove_all_in_range6(&list, 8, 12);
    int_sq_list_print(&list);
}

int main() {
    q5();
    q6();
    return 0;
}