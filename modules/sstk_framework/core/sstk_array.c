#include <sstk_core.h>

sstk_int_t sstk_array_init(sstk_array_t* arr, sstk_allocator_t* allocator,
                                         sstk_uint_t n, size_t size) {
    arr->count = 0;
    arr->size = size;
    arr->capacity = n;
    arr->allocator = allocator ? allocator : sstk_cycle->allocator;

    arr->items = arr->allocator->alloc_fn(n * size);
    if (arr->items == NULL) return SSTK_ERR;
    sstk_memzero(arr->items, n * size);

    return SSTK_OK;
}

sstk_array_t* sstk_array_create(sstk_allocator_t* a, sstk_uint_t n, size_t size) {
    sstk_array_t* arr;
    
    arr = a->alloc_fn(sizeof(sstk_array_t));
    if (arr == NULL) {
        return NULL;
    }

    if (sstk_array_init(arr, a, n, size) != SSTK_OK) {
        return NULL;
    }

    return arr;
}


void sstk_array_destroy(sstk_array_t* arr) {
    if (arr->items) {
        arr->allocator->dealloc_fn(arr->items);
    }
}


void* sstk_array_push(sstk_array_t* arr) {
    size_t size;
    void* new;
    void* item;

    if (arr->count == arr->capacity) {
        size = arr->size * arr->capacity;

        new = arr->allocator->alloc_fn(size * 2);
        if (new == NULL) 
            return NULL;
        sstk_memzero(new, size * 2);

        sstk_memcpy(new, arr->items, size);

        arr->allocator->dealloc_fn(arr->items);
        arr->items = new;
        arr->capacity *= 2;
    }

    item = (unsigned char*) arr->items + arr->size * arr->count;
    arr->count++;

    return item;
}


void* sstk_array_push_n(sstk_array_t* arr, sstk_uint_t n) {
    size_t size;
    void* new;
    void* item;
    sstk_uint_t capacity;

    size = arr->size * n;

    if (arr->count + n > arr->capacity) {
        capacity = 2 * ((n >= arr->capacity) ? n : arr->capacity);
        
        new = arr->allocator->alloc_fn(capacity * size);
        if (new == NULL) 
            return NULL;
        sstk_memzero(new, capacity * size);

        sstk_memcpy(new, arr->items, arr->capacity * arr->size);

        arr->allocator->dealloc_fn(arr->items);
        arr->items = new;
        arr->capacity = capacity;
    }

    item = (unsigned char*) arr->items + arr->size * arr->count;
    arr->count += n;

    return item;
}
