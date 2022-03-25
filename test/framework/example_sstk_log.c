/**
 * @file example_sstk_log.c
 *
 * @brief 日志输出器使用 示例
 */
#include <sstk_core.h>

static void test_log_writer(const char* buf, size_t size) { printf("%.*s", (int) size, buf); }

static sstk_logger_t logger = {
    .name = sstk_string("test"),    // 日志输出器名
    .level = SSTK_LOG_LEVEL_INFO,   // 日志输出等级 (>=level的输出)
    .flags = SSTK_LOG_FLAGS_ALL,    // 日志拼接标志
    .writer = test_log_writer       // 日志定向
};

int main() {
    sstk_log_debug(&logger, "测试DEBUG日志 %d", 1);
    sstk_log_info(&logger, "测试INFO日志 %d", 1);
    sstk_log_warn(&logger, "测试WARN日志 %d", 1);
    sstk_log_error(&logger, "测试ERROR日志 %d", 1);
    sstk_log_fatal(&logger, "测试FATAL日志 %d", 1);
    return 0;
}
