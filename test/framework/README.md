# sstk_framework 一些工具/数据结构的使用示例

## 使用 sstk_logger_t 进行日志定向打印

```c
static void test_log_writer(const char* buf, size_t size) { printf("%.*s", (int) size, buf); }

static sstk_logger_t logger = {
    .name = sstk_string("test"),    // 日志输出器名
    .level = SSTK_LOG_LEVEL_INFO,   // 日志输出等级 (>=level的输出)
    .flags = SSTK_LOG_FLAGS_ALL,    // 日志拼接标志
    .writer = test_log_writer       // 日志定向
};

int main() {
    // sstk_log_debug(logger, fmt, ...)
    // 等价于
    // sstk_log(logger, SSTK_LOG_DEBUG, fmt, ...)
    // 有些嵌入式系统编译器未提供c99标准语法的编译, 则不能使用关于__VA_ARGS__的宏定义
    sstk_log_debug(&logger, "测试DEBUG日志 %d", 1);
    sstk_log_info(&logger, "测试INFO日志 %d", 1);
    sstk_log_warn(&logger, "测试WARN日志 %d", 1);
    sstk_log_error(&logger, "测试ERROR日志 %d", 1);
    sstk_log_fatal(&logger, "测试FATAL日志 %d", 1);
    return 0;
}

```

## 使用 sstk_queue_t 实现双向链表

```c
typedef struct {
    int idx;
    const char* msg;
    sstk_queue_t queue;
} my_data_t;

int main() {
    my_data_t h = {0, "h", sstk_queue_initializer(&h.queue)};
    my_data_t e = {1, "e", sstk_queue_initializer(&e.queue)};
    my_data_t l = {2, "l", sstk_queue_initializer(&l.queue)};
    my_data_t l1 = {3, "l", sstk_queue_initializer(&l1.queue)};
    my_data_t o = {4, "o", sstk_queue_initializer(&o.queue)};
    my_data_t w = {5, "w", sstk_queue_initializer(&w.queue)};
    my_data_t o1 = {6, "o", sstk_queue_initializer(&o1.queue)};
    my_data_t r = {7, "r", sstk_queue_initializer(&r.queue)};
    my_data_t l2 = {8, "l", sstk_queue_initializer(&l2.queue)};
    my_data_t d = {9, "d", sstk_queue_initializer(&d.queue)};

    sstk_queue_t qheader = sstk_queue_initializer(&qheader);
    sstk_queue_insert_tail(&qheader, &h.queue);
    sstk_queue_insert_tail(&qheader, &e.queue);
    sstk_queue_insert_tail(&qheader, &l.queue);
    sstk_queue_insert_tail(&qheader, &l1.queue);
    sstk_queue_insert_tail(&qheader, &o.queue);
    sstk_queue_insert_tail(&qheader, &w.queue);
    sstk_queue_insert_tail(&qheader, &o1.queue);
    sstk_queue_insert_tail(&qheader, &r.queue);
    sstk_queue_insert_tail(&qheader, &l2.queue);
    sstk_queue_insert_tail(&qheader, &d.queue);
    
    // 游标指针
    sstk_queue_t* cursor;
    printf("asc:\n");
    // 正序遍历
    sstk_queue_foreach(cursor, &qheader) {
        // 根据地址偏移反向获取源数据
        my_data_t* data = sstk_queue_data(cursor, my_data_t, queue);

        printf("idx: %d, msg: %s\n", data->idx, data->msg);
    }
    
    printf("desc:\n");
    // 逆序遍历
    sstk_queue_rforeach(cursor, &qheader) {
        // 根据地址偏移反向获取源数据
        my_data_t* data = sstk_queue_data(cursor, my_data_t, queue);

        printf("idx: %d, msg: %s\n", data->idx, data->msg);
    }
    

    return 0;
}
```

## 使用 sstk_rbtree_t 红黑树用作高性能检索工具

```c
sstk_rbtree_t rbt;

sstk_str_rbtree_init(&rbt, NULL, NULL);
sstk_rbtree_str_if_t* str_rbt = (sstk_rbtree_str_if_t*) &rbt;

sstk_str_t s1 = sstk_string("s1");
str_rbt->put(str_rbt, &s1, (void*) 1L);
sstk_str_t s2 = sstk_string("s2");
str_rbt->put(str_rbt, &s2, (void*) 2L);
sstk_str_t s3 = sstk_string("s3");
str_rbt->put(str_rbt, &s3, (void*) 3L);
sstk_str_t s4 = sstk_string("s4");
str_rbt->put(str_rbt, &s4, (void*) 4L);

sstk_str_t s;
s.data = (unsigned char*) "s3";
s.len = 2;
void* ret;
str_rbt->get(str_rbt, &s, &ret);
sstk_uint_t iret = (sstk_uint_t) ret;

printf("s3 -> %d\n", (int) iret);

sstk_rbtree_free(&rbt);
```

## 使用 sstk_kdtree_t 高性能k维距离检索数据结构

```c
sstk_kdtree_opt_t opt = {
    .allocable = &sstk_default_allocable,
    .allocator = sstk_default_allocator,
    .dim = 3
};

sstk_kdtree_t* kdtree = sstk_kd_create(&opt);

sstk_kd_insert3(kdtree, 0, 0, 0, "no.0");
sstk_kd_insert3(kdtree, 10, 11, 12, "no.1");
sstk_kd_insert3(kdtree, 18, 14, 16, "no.2");
sstk_kd_insert3(kdtree, 4, 4, 4, "no.4");
sstk_kd_insert3(kdtree, 3, 5, 4, "no.5");
sstk_kd_insert3(kdtree, 2, 6, 2, "no.6");
sstk_kd_insert3(kdtree, 1, 7, 4, "no.7");

// 查询(0, 0, 0)点, 范围为7以内的所有数据
// ordered 按照 由近到远排序, 无ordered无排序
sstk_kdres_t* ret = sstk_kd_nearest_range3_ordered(kdtree, 0, 0, 0, 7);

// 遍历结果
for (; sstk_kd_res_has_next(ret); sstk_kd_res_next(ret)) {
    printf("cur %s\n", (char*) sstk_kd_res_item_data(ret));
}

// 释放结果集
sstk_kd_res_free(ret);

return 0;
```

## 使用 sstk_heap5 管理自定义堆内存

```c
static char heap_mem1[4096];
static char heap_mem2[2048];
static sstk_heap_region_t regions[] = {
    {
        .addr = heap_mem1, 
        .size = sizeof(heap_mem1)
    },  
    {
        .addr = heap_mem2, 
        .size = sizeof(heap_mem2)
    },
    { NULL, 0 } 
};

int main() {
    struct sstk_heap_stats_s stats;

    sstk_heap_init(regions);

    sstk_heap_get_stats(&stats);
    printf("初始剩余堆内存 %ld\n", stats.bytes_of_avalable_space);

    int* pint = sstk_heap_malloc(sizeof(int) * 100);

    sstk_heap_get_stats(&stats);
    printf("分配后剩余堆内存 %ld\n", stats.bytes_of_avalable_space);

    sstk_heap_free(pint);

    sstk_heap_get_stats(&stats);
    printf("释放后剩余堆内存 %ld\n", stats.bytes_of_avalable_space);

    return 0;
}
```

## 使用 sstk_thread_pool 线程池作为异步处理工具

> 前提: os层需要posix堆phread的api, eg. pthread_create等<br/>
> 几乎所有操作系统包括绝大多数嵌入式操作系统都提供了posix API, 如果出现了posix api未定义异常, 查阅文档关于posix api的使用方法一般都可以解决

```c
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
```

