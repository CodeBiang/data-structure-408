#include <sstk_core.h>

#if SSTK_ARCH == SSTK_ARCH_TI_OS

void sstk_localtime(time_t s, sstk_tm_t *tm) {
    sstk_tm_t *t;

    t = localtime(&s);
    *tm = *t;

    tm->sstk_tm_mon++;
    tm->sstk_tm_year += 1900;
}

void sstk_libc_localtime(time_t s, struct tm *tm) {
    struct tm *t;

    t = localtime(&s);
    *tm = *t;
}

void sstk_libc_gmtime(time_t s, struct tm *tm) {
    struct tm *t;

    t = gmtime(&s);
    *tm = *t;
}

void sstk_gettimeofday(struct timeval* tp) {
    /* 未实现 */
    tp->tv_sec = 0;
    tp->tv_usec = 0;
}

#endif
