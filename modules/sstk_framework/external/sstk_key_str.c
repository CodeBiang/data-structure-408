#include <sstk_core.h>

static int sstk_str_hash_code(void* p);
static int sstk_str_cmp(const void* p1, const void* p2);
static void* sstk_str_constructor(void* p, sstk_allocator_t* allocator);
static void sstk_str_destructor(void* p, sstk_allocator_t* allocator);

static sstk_allocable_t sstk_str_allocable = {
    .name = sstk_string("sstk_str_allocable"),
    .constructor = sstk_str_constructor,
    .destructor = sstk_str_destructor
};

sstk_key_base_t sstk_str_key_base = {
    .cmp = sstk_str_cmp,
    .allocable = &sstk_str_allocable,
    .hash_code = sstk_str_hash_code
};

struct sstk_hash_str_s {
    int hash;
    sstk_str_t str;
};

int sstk_str_hash_code(void* p) {
    size_t i;
    const char* val;
    struct sstk_hash_str_s* s;
    int h;

    s = (struct sstk_hash_str_s*) p;
    h = s->hash;
    
    if (h == 0 && s->str.len > 0) {
        val = (const char*) s->str.data;

        for (i = 0; i < s->str.len; i++) {
            h = 31 * h + val[i];
        }
        s->hash = h;
    }

    return h;
}

int sstk_str_cmp(const void* p1, const void* p2) {
    const struct sstk_hash_str_s *s1, *s2;
    size_t len1, len2, lim, k;
    const char *v1, *v2;
    char c1, c2;
    
    if (p1 == p2) return 0;

    s1 = (const struct sstk_hash_str_s*) p1;
    s2 = (const struct sstk_hash_str_s*) p2;

    len1 = s1->str.len;
    len2 = s2->str.len;
    lim = sstk_min(len1, len2);
    v1 = (const char*) s1->str.data;
    v2 = (const char*) s2->str.data;

    k = 0;
    while (k < lim) {
        c1 = v1[k];
        c2 = v2[k];
        if (c1 != c2) {
            return c1 - c2;
        }
        k++;
    }
    return len1 - len2;
}

void* sstk_str_constructor(void* p, sstk_allocator_t* allocator) {
    const sstk_str_t* s = (const sstk_str_t*) p;
    struct sstk_hash_str_s* str = allocator->alloc_fn(sizeof(struct sstk_hash_str_s));

    str->str.data = s->data;
    str->str.len = s->len;
    str->hash = 0;

    return str;
}

void sstk_str_destructor(void* p, sstk_allocator_t* allocator) {
    allocator->dealloc_fn(p);
}

