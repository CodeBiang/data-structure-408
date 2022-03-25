#include <sstk_core.h>

void sstk_strlow(unsigned char* dst, unsigned char* src, size_t n) {
    while (n) {
        *dst = sstk_tolower(*src);
        dst++;
        src++;
        n--;
    }
}

size_t sstk_strnlen(unsigned char* p, size_t n) {
    size_t i;

    for (i = 0; i < n; i++) {
        if (p[i] == '\0') {
            return i;
        }
    }

    return n;
}

unsigned char* sstk_cpystrn(unsigned char* dst, unsigned char* src, size_t n) {
    if (n == 0) {
        return dst;
    }

    while (--n) {
        *dst = *src;

        if (*dst == '\0') {
            return dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    return dst;
}

sstk_int_t sstk_strcasecmp(unsigned char* s1, unsigned char* s2) {
    sstk_uint_t c1, c2;
    for (;;) {
        c1 = (sstk_uint_t)*s1++;
        c2 = (sstk_uint_t)*s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        if (c1 == c2) {
            if (c1) {
                continue;
            }

            return 0;
        }

        return c1 - c2;
    }
}

sstk_int_t sstk_strncasecmp(unsigned char* s1, unsigned char* s2, size_t n) {
    sstk_uint_t c1, c2;

    while (n) {
        c1 = (sstk_uint_t)*s1++;
        c2 = (sstk_uint_t)*s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        if (c1 == c2) {
            if (c1) {
                n--;
                continue;
            }

            return 0;
        }

        return c1 - c2;
    }

    return 0;
}

unsigned char* sstk_strnstr(unsigned char* s1, char* s2, size_t len) {
    unsigned char c1, c2;
    size_t n;

    c2 = *(unsigned char*)s2++;

    n = sstk_strlen(s2);

    do {
        do {
            if (len-- == 0) {
                return NULL;
            }

            c1 = *s1++;

            if (c1 == 0) {
                return NULL;
            }

        } while (c1 != c2);

        if (n > len) {
            return NULL;
        }

    } while (sstk_strncmp(s1, (unsigned char*)s2, n) != 0);

    return --s1;
}

unsigned char* sstk_strstrn(unsigned char* s1, char* s2, size_t n) {
    unsigned char c1, c2;

    c2 = *(unsigned char*)s2++;

    do {
        do {
            c1 = *s1++;

            if (c1 == 0) {
                return NULL;
            }

        } while (c1 != c2);

    } while (sstk_strncmp(s1, (unsigned char*)s2, n) != 0);

    return --s1;
}
unsigned char* sstk_strcasestrn(unsigned char* s1, char* s2, size_t n) {
    sstk_uint_t c1, c2;

    c2 = (sstk_uint_t)*s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    do {
        do {
            c1 = (sstk_uint_t)*s1++;

            if (c1 == 0) {
                return NULL;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } while (c1 != c2);

    } while (sstk_strncasecmp(s1, (unsigned char*)s2, n) != 0);

    return --s1;
}
unsigned char* sstk_strlcasestrn(unsigned char* s1, unsigned char* last, unsigned char* s2, size_t n) {
    sstk_uint_t c1, c2;

    c2 = (sstk_uint_t)*s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
    last -= n;

    do {
        do {
            if (s1 >= last) {
                return NULL;
            }

            c1 = (sstk_uint_t)*s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } while (c1 != c2);

    } while (sstk_strncasecmp(s1, s2, n) != 0);

    return --s1;
}

void sstk_sort(void* base, size_t n, size_t size,
               sstk_int_t (*cmp)(const void*, const void*)) {
    unsigned char *p1, *p2, *p;

    p = sstk_cycle->allocator->alloc_fn(size);
    if (p == NULL) {
        return;
    }

    for (p1 = (unsigned char*)base + size; p1 < (unsigned char*)base + n * size; p1 += size) {
        sstk_memcpy(p, p1, size);

        for (p2 = p1; p2 > (unsigned char*)base && cmp(p2 - size, p) > 0; p2 -= size) {
            sstk_memcpy(p2, p2 - size, size);
        }

        sstk_memcpy(p2, p, size);
    }

    sstk_cycle->allocator->dealloc_fn(p);
}
