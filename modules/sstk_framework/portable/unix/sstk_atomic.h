#ifndef __SSTK_ATOMIC_H_
#define __SSTK_ATOMIC_H_

#include <sstk_core.h>

typedef int32_t sstk_atomic_int_t;
typedef uint32_t sstk_atomic_uint_t;
typedef volatile sstk_atomic_uint_t sstk_atomic_t;

static inline sstk_atomic_uint_t sstk_atomic_cmp_set(sstk_atomic_t* lock, sstk_atomic_uint_t old,
                                                     sstk_atomic_uint_t set) {
    if (*lock == old) {
        *lock = set;
        return 1;
    }

    return 0;
}

static inline sstk_atomic_int_t sstk_atomic_fetch_add(sstk_atomic_t* value, sstk_atomic_int_t add) {
    sstk_atomic_int_t old;

    old = *value;
    *value += add;

    return old;
}

#define sstk_memory_barrier()
#define sstk_cpu_pause()

#define sstk_trylock(lock) (*(lock) == 0 && sstk_atomic_cmp_set(lock, 0, 1))
#define sstk_unlock(lock) *(lock) = 0

#endif
