#ifndef __SSTK_TIME_H_
#define __SSTK_TIME_H_

#include <sstk_core.h>

typedef struct tm sstk_tm_t;

#define sstk_tm_sec            tm_sec
#define sstk_tm_min            tm_min
#define sstk_tm_hour           tm_hour
#define sstk_tm_mday           tm_mday
#define sstk_tm_mon            tm_mon
#define sstk_tm_year           tm_year
#define sstk_tm_wday           tm_wday
#define sstk_tm_isdst          tm_isdst

#define sstk_tm_gmtoff         tm_gmtoff
#define sstk_tm_zone           tm_zone

#define sstk_tm_sec_t          int
#define sstk_tm_min_t          int
#define sstk_tm_hour_t         int
#define sstk_tm_mday_t         int
#define sstk_tm_mon_t          int
#define sstk_tm_year_t         int
#define sstk_tm_wday_t         int

void sstk_timezone_update(void);
void sstk_localtime(time_t s, sstk_tm_t *tm);
void sstk_libc_localtime(time_t s, struct tm *tm);
void sstk_libc_gmtime(time_t s, struct tm *tm);

#define sstk_gettimeofday(tp)  (void) gettimeofday(tp, NULL);
#define sstk_msleep(ms)        (void) usleep(ms * 1000)
#define sstk_sleep(s)          (void) sleep(s)

#endif
