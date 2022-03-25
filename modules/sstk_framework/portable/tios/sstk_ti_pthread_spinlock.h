/**
 * @file    sstk_ti_pthread_spinlock.h
 * @date    2022/1/10
 * @author  赵陈淏
 * @brief   简化版本的自旋锁, 未实现cpu级的锁自旋
 */
#ifndef __SSTK_TI_PTHREAD_SPINLOCK_H_
#define __SSTK_TI_PTHREAD_SPINLOCK_H_

#define __pthread_spinlock_locked(__lock) ((*__lock) == 1)

#define PTHREAD_PROCESS_SHARED 1

typedef volatile int pthread_spinlock_t;

static inline int pthread_spin_init(pthread_spinlock_t* __lock, int __pshared) {
    (void) __pshared;
    *__lock = 0;
    return 0;
}

/* Destroy the spinlock LOCK.  */
static inline int pthread_spin_destroy(pthread_spinlock_t* __lock) {
    (void) __lock;
    return 0;
}

/* Wait until spinlock LOCK is retrieved.  */
static inline int pthread_spin_lock(pthread_spinlock_t* __lock) {
    // 未释放锁忙等
    while (__pthread_spinlock_locked(__lock));
    // 上锁
    *__lock = 1;
    return 0;
}

/* Try to lock spinlock LOCK.  */
static inline int pthread_spin_trylock(pthread_spinlock_t* __lock) {
    if (!__pthread_spinlock_locked(__lock)) *__lock = 1;
    else return -1;
    return 0;
}

/* Release spinlock LOCK.  */
static inline int pthread_spin_unlock(pthread_spinlock_t* __lock) {
    *__lock = 0;
    return 0;
}

#endif
