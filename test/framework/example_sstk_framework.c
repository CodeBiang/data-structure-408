/**
 * @file example_sstk_framework.c
 *
 * @brief 数据处理框架调用实例
 */
#include <sstk_core.h>

static sstk_int_t init_chain_handler(sstk_chain_t* chain);
static sstk_int_t init_poll_handler(sstk_chain_t* chain);
static sstk_int_t exit_poll_handler(sstk_chain_t* chain);
static sstk_int_t process_handler(sstk_chain_t* chain);
static void test_log_writer(const char* buf, size_t size) { printf("%.*s", (int) size, buf); }

static volatile sstk_bool_t app_shutdown = false;

static sstk_logger_t logger = {
    .name = sstk_string("test"),   // 日志输出器名
    .level = SSTK_LOG_LEVEL_INFO,  // 日志输出等级 (>=level的输出)
    .flags = SSTK_LOG_FLAGS_ALL,   // 日志拼接标志
    .writer = test_log_writer      // 日志定向
};


// 示例的负载数据, 实际使用灵活处理
typedef struct {
    int data[20];
} test_payload_t;
static test_payload_t payload = {{0}};

static sstk_chain_t chain1 = {
    .name = sstk_string("test1"),
    .version = 1,
    .type = SSTK_CORE_CHAIN,
    .expire = -1,
    .init_chain = init_chain_handler,
    .init_poll = init_poll_handler,
    .exit_poll = exit_poll_handler,
    .poll = process_handler,
    .ctx = &payload
};

static sstk_chain_t chain2 = {
    .name = sstk_string("test2"),
    .version = 1,
    .type = SSTK_CORE_CHAIN,
    .expire = -1,
    .init_poll = init_poll_handler,
    .exit_poll = exit_poll_handler,
    .poll = process_handler,
    .ctx = &payload
};

static sstk_chain_t chain3 = {
    .name = sstk_string("test3"),
    .version = 1,
    .type = SSTK_CORE_CHAIN,
    .expire = -1,
    .init_poll = init_poll_handler,
    .exit_poll = exit_poll_handler,
    .poll = process_handler,
    .ctx = &payload
};

int main() {
    static sstk_cycle_t* cycle;
    static sstk_cycle_t init_cycle;

    sstk_memzero(&init_cycle, sizeof(sstk_cycle_t));

    cycle = sstk_bootstrap
                ->with_cycle(sstk_bootstrap, &init_cycle)                   // 使用初始化cycle
                ->with_allocator(sstk_bootstrap, sstk_default_allocator)    // 必须指定内存分配器
                ->add_logger(sstk_bootstrap, &logger)                       // 添加日志记录器
                ->init_cycle(sstk_bootstrap)                                // 初始化
                ->add_chain(sstk_bootstrap, &chain1)                        // 添加数据处理链1
                ->add_chain(sstk_bootstrap, &chain2)                        // 添加数据处理链2
                ->add_chain(sstk_bootstrap, &chain3)                        // 添加数据处理链3
                ->setup(sstk_bootstrap);                                    // 装载


    while (!app_shutdown) {
        sstk_msleep(500);
        sstk_cycle_poll(cycle);
    }

    sstk_exit_cycle();

    return 0;
}

static sstk_int_t init_chain_handler(sstk_chain_t* chain) {
    test_payload_t* p = chain->ctx;
    sstk_log_info(chain->cycle->logger, "链节点 [%s] 正在初始化", chain->name.data);

    time_t t = time(NULL);
    for (int i = 0; i < 20; i++) {
        srand(t + i);
        p->data[i] = rand() % 100;
    }
}

static sstk_int_t init_poll_handler(sstk_chain_t* chain) {
    sstk_log_info(chain->cycle->logger, "链节点 [%s] 处理数据前", chain->name.data);

    return SSTK_OK;
    
}

static sstk_int_t exit_poll_handler(sstk_chain_t* chain) {
    sstk_log_info(chain->cycle->logger, "链节点 [%s] 处理数据后", chain->name.data);

    return SSTK_OK;
}

static sstk_int_t process_handler(sstk_chain_t* chain) {
    test_payload_t* p = chain->ctx;
    sstk_log_info(chain->cycle->logger, "链节点 [%s] 正在处理数据", chain->name.data);

    for (int i = 0; i < 20; i++) {
        printf("%d ", p->data[i]);
    }
    printf("\n");

    // 获取上个节点
    sstk_chain_t* chain_prev = sstk_chain_prev(chain);
    
    if (chain_prev) {
        printf("上个节点是 [%s]\n", chain_prev->name.data);
    }

    return SSTK_OK;
}
