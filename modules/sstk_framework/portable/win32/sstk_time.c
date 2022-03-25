#include <sstk_core.h>

#if SSTK_ARCH == SSTK_ARCH_WIN32
#include <windows.h>

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
    time_t t;
    SYSTEMTIME wtm;
    struct tm tm;

    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900;
    tm.tm_mon = wtm.wMonth - 1;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = 1;

    t = mktime(&tm);

    tp->tv_sec = t;
    tp->tv_usec = wtm.wMilliseconds * 1000;
}

void sstk_msleep(long ms) {
    Sleep(ms);
}

#endif
