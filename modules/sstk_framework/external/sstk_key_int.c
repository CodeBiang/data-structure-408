#include <sstk_core.h>


static int sstk_int_hash_code(void* p);
static int sstk_int_cmp(const void* p1, const void* p2);
static void* sstk_int_constructor(void* p, sstk_allocator_t* allocator);
static void sstk_int_destructor(void* p, sstk_allocator_t* allocator);

static sstk_allocable_t sstk_int_allocable = {
    .name = sstk_string("sstk_int_allocable"),
    .constructor = sstk_int_constructor,
    .destructor = sstk_int_destructor
};

sstk_key_base_t sstk_int_key_base = {
    .cmp = sstk_int_cmp,
    .allocable = &sstk_int_allocable,
    .hash_code = sstk_int_hash_code
};

int sstk_int_hash_code(void* p) {
    return *((const int*) p);
}

int sstk_int_cmp(const void* p1, const void* p2) {
    int v1 = *(const int*) p1;
    int v2 = *(const int*) p2;
    return v1 - v2;
}

void* sstk_int_constructor(void* p, sstk_allocator_t* allocator) {
    int v = *(const int*) p;
    int* pv = allocator->alloc_fn(sizeof(int));
    *pv = v;
    return (void*) pv;
}

void sstk_int_destructor(void* p, sstk_allocator_t* allocator) {
    allocator->dealloc_fn(p);
}
