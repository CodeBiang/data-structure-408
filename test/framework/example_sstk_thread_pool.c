/**
 * @file example_sstk_log.c
 *
 * @brief 线程池 示例
 */
#include <sstk_core.h>
#include <sstk_thread_pool.h>

typedef struct {
    int task_id;
    int sleep_time_before;
    int sleep_time_after;
} my_data_t;

static void test_log_writer(const char* buf, size_t size) { printf("%.*s", (int) size, buf); }

static sstk_logger_t logger = {
    .name = sstk_string("test"),   // 日志输出器名
    .level = SSTK_LOG_LEVEL_INFO,  // 日志输出等级 (>=level的输出)
    .flags = SSTK_LOG_FLAGS_ALL,   // 日志拼接标志
    .writer = test_log_writer      // 日志定向
};

static void* task(void* data) {
    my_data_t* my_data = (my_data_t*) data;

    sleep(my_data->sleep_time_before);

    sstk_log_info(&logger, "%d run", my_data->task_id);

    sleep(my_data->sleep_time_after);

    sstk_log_info(&logger, "%d done", my_data->task_id);

    return NULL;
}

int main() {
    // 构建一个核心线程数量为4， 最大队列长度为8的线程池
    sstk_thread_pool_t* pool = sstk_thread_pool_create(&logger, 4, 8);

    // 第2个任务 打印前睡眠1s, 打印后睡眠3s
    sstk_thread_pool_submit(pool, task, (my_data_t[]){{1, 1, 3}}, NULL);
    // 第2个任务
    sstk_thread_pool_submit(pool, task, (my_data_t[]){{2, 2, 2}}, NULL);
    // 第3个任务
    sstk_thread_pool_submit(pool, task, (my_data_t[]){{3, 3, 2}}, NULL);
    // 第4个任务
    sstk_thread_pool_submit(pool, task, (my_data_t[]){{4, 4, 2}}, NULL);
    // 第5个任务 因为核心线程已满, 在 以上任务任何一个任务执行完成后才能执行
    sstk_thread_pool_submit(pool, task, (my_data_t[]){{5, 0, 5}}, NULL);

    sleep(15);
    return 0;
}
