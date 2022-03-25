/**
 * @file
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief
 */
#ifndef __SSTK_FRAMEWORK_CONF_H_
#define __SSTK_FRAMEWORK_CONF_H_

#include <sstk_portable_headers.h>

#ifndef bool
#define bool int
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#ifndef offsetof
#define offsetof(STRUCTURE, FIELD) ((int) ((char *) &((STRUCTURE *) 0)->FIELD))
#endif

typedef intptr_t sstk_int_t;
typedef uintptr_t sstk_uint_t;
typedef intptr_t sstk_flag_t;
typedef bool sstk_bool_t;

#ifndef SSTK_ALIGNMENT
#define SSTK_ALIGNMENT   sizeof(unsigned long)    /* platform word */
#endif

#define SSTK_MAX_UINT32_VALUE  (uint32_t) 0xffffffff
#define SSTK_MAX_INT32_VALUE   (uint32_t) 0x7fffffff

#define sstk_align(d, a) (((d) + (a - 1)) & ~(a - 1))
#define sstk_align_ptr(p, a) \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

/*!-- 启用快速日志API [需要C99/GNU99+] --*/
#define SSTK_FRAMEWROK_ENABLE_LOGGER_VA_ARG_API 1

/*!-- 单条日志大小限制 --*/
#define SSTK_FRAMEWORK_LOG_BUF_SIZE 1024

#define SSTK_FRAMEWORK_LOG_MAX_COUNT 4

#endif
