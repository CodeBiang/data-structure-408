#include <sstk_core.h>

static int sstk_default_cmp(const void* p1, const void* p2) {
    sstk_uint_t pi1 = (sstk_uint_t) p1;
    sstk_uint_t pi2 = (sstk_uint_t) p2;
    return pi1 - pi2;
}

static int sstk_default_hash_code(void* p) {
    sstk_uint_t pi = (sstk_uint_t) p;
    return (int) (pi ^ (pi >> 32));
}

static void* sstk_default_constructor(void* p, sstk_allocator_t* allocator) { return p; }

static void sstk_default_destructor(void* p, sstk_allocator_t* allocator) {}

sstk_allocable_t sstk_default_allocable = {
    .constructor = sstk_default_constructor,
    .destructor = sstk_default_destructor
};

sstk_key_base_t sstk_default_key_base = {
    .cmp = sstk_default_cmp,
    .hash_code = sstk_default_hash_code,
    .allocable = &sstk_default_allocable
};
