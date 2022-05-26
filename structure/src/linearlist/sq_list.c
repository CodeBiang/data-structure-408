#include "sq_list.h"

#include <stdlib.h>
#include <string.h>

bool sq_list_init(sq_list_t* l, size_t elem_size, size_t capacity) {
    memset(l, 0, sizeof(sq_list_t));

    l->item_size = elem_size;
    l->capacity = capacity;
    l->length = 0;
    l->data = malloc(capacity * elem_size);

    return l->data != NULL;
}

void sq_list_destory(sq_list_t* l) {
    if (l->data) free(l->data);
}

void* sq_list_push(sq_list_t* l) {
    void* ret;

    if (l->length < (int) l->capacity) {
        ret = (void*) ((char*) l->data + l->item_size * l->length);
        l->length++;
        return ret;
    }

    return NULL;
}

bool sq_list_insert(sq_list_t* l, int idx, const void* elem) {
    if (l->length >= (int) l->capacity) return false;

    if (idx < 0 || idx > l->length) return false;

    memmove(((char*) l->data) + l->item_size * (idx + 1), 
            ((char*) l->data) + l->item_size * idx,
            l->item_size * (l->length - idx));

    memcpy(((char*) l->data) + l->item_size * idx, elem, l->item_size);

    l->length++;

    return true;
}

bool sq_list_delete(sq_list_t* l, int idx, void* elem) {
    if (idx < 0 || idx > l->length) return false;

    if (elem) memcpy(elem, (char*) l->data + idx * l->item_size, l->item_size);

    memmove(((char*) l->data) + l->item_size * idx, 
            ((char*) l->data) + l->item_size * (idx + 1),
            l->item_size * (l->length - idx - 1));

    l->length--;

    return true;
}

int sq_list_index_of(sq_list_t* l, const void* elem, bool (*equals)(const void*, const void*)) {
    int ret = 0;
    void* pend = (char*) l->data + (l->length - 1) * l->item_size;
    void* pcur = l->data;

    while (pcur != pend) {
        if (equals(pcur, elem)) 
            return ret;
        
        ret++;
        pcur = (char*) pcur + l->item_size;
    }
    
    return -1;
}
