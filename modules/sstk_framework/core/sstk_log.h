/**
 * @file    sstk_log.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   日志实现
 *
 * @todo    需要实现时间接口
 * @todo    池化buffer可以提高少许性能
 */
#ifndef __SSTK_LOG_H_
#define __SSTK_LOG_H_

#include <sstk_core.h>

#if defined __GNUC__
#define SLOG_CHECK_PRINTF(m, n) __attribute__((format(printf, m, n)))
#else
#define SLOG_CHECK_PRINTF(m, n)
#endif

enum sstk_log_level {
    SSTK_LOG_LEVEL_DEBUG = 20,
    SSTK_LOG_LEVEL_INFO = 40,
    SSTK_LOG_LEVEL_NOTICE = 60,
    SSTK_LOG_LEVEL_WARN = 80,
    SSTK_LOG_LEVEL_ERROR = 100,
    SSTK_LOG_LEVEL_FATAL = 120
};

typedef void (*sstk_log_writer_fn)(const char* buf, size_t size);

/* 日志记录器 */
struct sstk_logger_s {
    sstk_str_t name; /* 日志记录器名称 */

    sstk_int_t level; /* 日志等级 */

    sstk_log_writer_fn writer; /* 日志写入/输出回调 */

    sstk_flag_t flags;

    sstk_logger_t* next; /* 下一个日志记录器 */
};

sstk_int_t sstk_add_logger(sstk_cycle_t* cycle, sstk_logger_t* logger);

/**
 * 日志发送器
 *
 *
 * @param file 文件名__FILE__
 * @param filelen 文件名长度
 * @param func 函数名__func__
 * @param line 行数__LINE__
 * @param level 日志等级
 * @param format 格式
 * @param ... 变参
 */
void sstk_log(sstk_logger_t* logger, const char* file, size_t filelen, const char* func,
              size_t funclen, long line, int level, const char* format, ...)
    SLOG_CHECK_PRINTF(8, 9);

#define SSTK_LOG_PREFIX __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__,
#define SSTK_LOG_FATAL SSTK_LOG_PREFIX SSTK_LOG_LEVEL_FATAL
#define SSTK_LOG_ERROR SSTK_LOG_PREFIX SSTK_LOG_LEVEL_ERROR
#define SSTK_LOG_WARN SSTK_LOG_PREFIX SSTK_LOG_LEVEL_WARN
#define SSTK_LOG_NOTICE SSTK_LOG_PREFIX SSTK_LOG_LEVEL_NOTICE
#define SSTK_LOG_INFO SSTK_LOG_PREFIX SSTK_LOG_LEVEL_INFO
#define SSTK_LOG_DEBUG SSTK_LOG_PREFIX SSTK_LOG_LEVEL_DEBUG

#define SSTK_LOG_FLAG_NAME 0x01
#define SSTK_LOG_FLAG_TIME 0x02
#define SSTK_LOG_FLAG_FILE 0x04
#define SSTK_LOG_FLAG_FUNC 0x08
#define SSTK_LOG_FLAG_LEVEL 0x10
#define SSTK_LOG_FLAG_COLOR 0x20

#define SSTK_LOG_FLAGS_ALL                                                             \
    SSTK_LOG_FLAG_TIME | SSTK_LOG_FLAG_FILE | SSTK_LOG_FLAG_FUNC | SSTK_LOG_FLAG_LEVEL \
        | SSTK_LOG_FLAG_COLOR

#if SSTK_FRAMEWROK_ENABLE_LOGGER_VA_ARG_API

/*!-- fatal 日志 --*/
#define sstk_log_fatal(logger, fmt, ...) sstk_log(logger, SSTK_LOG_FATAL, fmt, ##__VA_ARGS__)

/*!-- error 日志 --*/
#define sstk_log_error(logger, fmt, ...) sstk_log(logger, SSTK_LOG_ERROR, fmt, ##__VA_ARGS__)

/*!-- warn 日志 --*/
#define sstk_log_warn(logger, fmt, ...) sstk_log(logger, SSTK_LOG_WARN, fmt, ##__VA_ARGS__)

/*!-- notice 日志 --*/
#define sstk_log_notice(logger, fmt, ...) sstk_log(logger, SSTK_LOG_NOTICE, fmt, ##__VA_ARGS__)

/*!-- info 日志 --*/
#define sstk_log_info(logger, fmt, ...) sstk_log(logger, SSTK_LOG_INFO, fmt, ##__VA_ARGS__)

/*!-- debug 日志 --*/
#define sstk_log_debug(logger, fmt, ...) sstk_log(logger, SSTK_LOG_DEBUG, fmt, ##__VA_ARGS__)

#endif

#endif
