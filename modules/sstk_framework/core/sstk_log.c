#include <sstk_core.h>

#define LOG_COLOR_START "\e["
#define LOG_COLOR_END "\e[0m"
#define LOG_COLOR_FATAL "0;31m"
#define LOG_COLOR_ERROR "0;35m"
#define LOG_COLOR_WARN "0;33m"
#define LOG_COLOR_NOTICE "0;34m"
#define LOG_COLOR_INFO "0;32m"
#define LOG_COLOR_DEBUG "0;36m"

#define LOG_COLORABLE(__colorable, __color, __fmt) \
    (__colorable ? (LOG_COLOR_START LOG_COLOR_##__color __fmt LOG_COLOR_END) )

#define SSTK_LOG_LEVEL_MAP(XX) \
    XX(FATAL)                  \
    XX(ERROR)                  \
    XX(WARN)                   \
    XX(INFO)                   \
    XX(NOTICE)                 \
    XX(DEBUG)

static inline const char* sstk_log_color(int level) {
    switch (level) {
#define XX(name)                \
    case SSTK_LOG_LEVEL_##name: \
        return LOG_COLOR_START LOG_COLOR_##name;
        SSTK_LOG_LEVEL_MAP(XX)
#undef XX
        default:
            return "";
    }
}

static inline const char* sstk_log_level_name(int level) {
    switch (level) {
#define XX(name)                \
    case SSTK_LOG_LEVEL_##name: \
        return #name;
        SSTK_LOG_LEVEL_MAP(XX)
#undef XX
        default:
            return "UNKNOWN";
    }
}

void sstk_log(sstk_logger_t* logger, const char* file, size_t filelen, const char* func,
              size_t funclen, long line, int level, const char* format, ...) {
    va_list ap;
    char buf[SSTK_FRAMEWORK_LOG_BUF_SIZE] = {0};
    struct tm tm;
    size_t size = sizeof(buf) - 2;
    size_t len, offset = 0;
    struct timeval tv;
    sstk_uint_t msec;
    sstk_bool_t colorable;

    sstk_gettimeofday(&tv);
    sstk_localtime(tv.tv_sec, &tm);

    msec = tv.tv_usec / 1000;

#define truncated_chk(len, size) \
    if (len >= size)             \
        goto truncated;          \
    else {                       \
        offset += len;           \
        size -= len;             \
    }

    while (logger) {
        if (!logger->writer || level < logger->level) goto next;

        colorable = logger->flags & SSTK_LOG_FLAG_COLOR;

        if (logger->flags & SSTK_LOG_FLAG_TIME) {
            len = snprintf(buf + offset, size, "%04d-%02d-%02d %02d:%02d:%02d.%03d  ", tm.tm_year,
                           tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, (int) msec);
            truncated_chk(len, size);
        }

        if (logger->flags & SSTK_LOG_FLAG_LEVEL) {
            if (colorable) {
                len = snprintf(buf + offset, size, "%s%-6s %s", sstk_log_color(level),
                               sstk_log_level_name(level), LOG_COLOR_END);
            } else {
                len = snprintf(buf + offset, size, "%6s ", sstk_log_level_name(level));
            }
            truncated_chk(len, size);
        }

        if (logger->flags & SSTK_LOG_FLAG_NAME) {
            len = snprintf(buf + offset, size, "%.*s ", (int) logger->name.len,
                           (const char*) logger->name.data);
            truncated_chk(len, size);
        }

        if (logger->flags & SSTK_LOG_FLAG_FILE) {
            len = snprintf(buf + offset, size, "--- %.*s#%ld %.*s ", (int) filelen, file, line,
                           (int) funclen, func);
            truncated_chk(len, size);
        }

        len = snprintf(buf + offset, size, ": ");
        truncated_chk(len, size);

        va_start(ap, format);
        // 性能瓶颈
        len = vsnprintf(buf + offset, size, format, ap);
        truncated_chk(len, size) va_end(ap);

    truncated:

        offset += snprintf(buf + offset, size, "\r\n");

        logger->writer(buf, offset);

    next:
        logger = logger->next != logger ? logger->next : NULL;
    }
}

sstk_int_t sstk_add_logger(sstk_cycle_t* cycle, sstk_logger_t* logger) {
    sstk_int_t count = 0;
    sstk_logger_t* cur;

    if (!cycle->logger) {
        cycle->logger = logger;
    } else {
        cur = cycle->logger;

        while (cur->next) {
            cur = cur->next;
            count++;
        }

        if (count >= SSTK_FRAMEWORK_LOG_MAX_COUNT) {
            return SSTK_OVERSIZE;
        }
        cur->next = logger;
    }

    return SSTK_OK;
}
