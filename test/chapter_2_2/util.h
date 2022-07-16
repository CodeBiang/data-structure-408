#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>

#include "sq_list.h"

static inline void sq_list_build(sq_list_t* list, int* arr, size_t size) {
    sq_list_init(list, sizeof(int), size);

    for (size_t i = 0; i < size; i++) {
        sq_list_at(list, i, int) = arr[i];
    }

    list->length = (int) size;
}

static inline void int_sq_list_print(const sq_list_t* list) {
    for (int i = 0; i < list->length; i++) {
        printf("%d ", sq_list_at(list, i, int));
    }
    printf("\n");
}

#endif
