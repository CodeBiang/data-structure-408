/**
 *
 * 7. 将两个有序顺序表合并为一个新的有序顺序表
 *
 *
 */
#include "util.h"

#include <stdlib.h>

static sq_list_t* sq_list_combine(const sq_list_t* l1, const sq_list_t* l2) {
    sq_list_t* ret = (sq_list_t*) malloc(sizeof(sq_list_t));

    sq_list_init(ret, sizeof(int), l1->length + l2->length);

    ret->length = l1->length + l2->length;
    int* arr = (int*) ret->data;
    const int* arr1 = (const int*) l1->data;
    const int* arr2 = (const int*) l2->data;

    int i, j;
    for (i = 0, j = 0; i < l1->length && j < l2->length; ) {
        if (arr1[i] < arr2[j]) {
            arr[i + j] = arr1[i];
            i++;
        } else {
            arr[i + j] = arr2[j];
            j++;
        }
    }
    
    while (i < l1->length) {
        arr[i + j] = arr1[i];
        i++;
    }

    while (j < l2->length) {
        arr[i + j] = arr2[j];
        j++;
    }

    return ret;
}

int main() {
    sq_list_t l1, l2;
    sq_list_t* l3;

    sq_list_build(&l1, (int[]){1, 2, 3, 4, 5, 21, 22, 23, 24, 25}, 10);
    sq_list_build(&l2, (int[]){7, 9, 16, 20, 24, 27, 28, 29, 30, 31}, 10);
    int_sq_list_print(&l1);
    int_sq_list_print(&l2);

    l3 = sq_list_combine(&l1, &l2);
    int_sq_list_print(l3);

    sq_list_destory(&l1);
    sq_list_destory(&l2);
    sq_list_destory(l3);
    free(l3);
    return 0;
}