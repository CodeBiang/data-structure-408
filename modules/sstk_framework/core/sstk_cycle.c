#include <sstk_core.h>

static sstk_allocator_t static_std_allocator = {
    .alloc_fn = malloc,
    .dealloc_fn = free,
    .realloc_fn = realloc
};

static void static_std_writer(const char* b, size_t len) {
    printf("%.*s", (int) len, b);
}

static sstk_logger_t static_stdout_logger = {
    .flags = SSTK_LOG_FLAGS_ALL,
    .level = SSTK_LOG_LEVEL_DEBUG,
    .name = sstk_string("stdout"),
    .writer = static_std_writer
};

static sstk_cycle_t default_cycle = {
    .conf_post_queue = sstk_queue_initializer(&default_cycle.conf_post_queue),
    .queue = sstk_queue_initializer(&default_cycle.queue),
    .logger = &static_stdout_logger,
    .allocator = &static_std_allocator
};

sstk_allocator_t* sstk_default_allocator = &static_std_allocator;
volatile sstk_cycle_t* sstk_cycle = &default_cycle;

sstk_int_t sstk_init_cycle(sstk_cycle_t* cycle) {
    cycle = cycle == NULL ? &default_cycle : cycle;
    cycle->chain_map = cycle->allocator->alloc_fn(sizeof(sstk_hash_map_t));

    if (!cycle->chain_map) {
        return SSTK_ERR;
    }

    // 初始化hashmap
    if (sstk_hash_map_default_init(cycle->chain_map, cycle->allocator) != SSTK_OK)
        return SSTK_ERR;

    sstk_queue_init(&cycle->queue);
    sstk_queue_init(&cycle->conf_post_queue);

    return SSTK_OK;
}

sstk_int_t sstk_exit_cycle() {
    sstk_queue_t* q;
    sstk_chain_t* c;
    volatile sstk_cycle_t* cycle = sstk_cycle;

    if (cycle == NULL) return SSTK_ABORT;

    

    while (!sstk_queue_empty(&cycle->queue)) {
        // 出队
        q = sstk_queue_head(&cycle->queue);
        sstk_queue_remove(q);
        
        c = sstk_queue_data(q, sstk_chain_t, queue);

        sstk_req_nonnull(c->exit_cycle)(c);
    }
    return SSTK_OK;
}

sstk_int_t sstk_setup_cycle(sstk_cycle_t* cycle) {
    sstk_int_t ret;

    ret = sstk_init_chains(cycle);
    if (ret != SSTK_OK) {
        return ret;
    }
    
    return ret;
}


void sstk_cycle_poll(sstk_cycle_t* cycle) {
    sstk_queue_t* q;
    sstk_chain_t* chain;
    sstk_conf_post_t* post;

    // TODO 此处应加入自旋锁保证线程安全

    // 配置遍历
    while (!sstk_queue_empty(&cycle->conf_post_queue)) {

        // 出队
        q = sstk_queue_head(&cycle->conf_post_queue);
        sstk_queue_remove(q);
        
        post = sstk_queue_data(q, sstk_conf_post_t, queue);
        
        // 查找链
        chain = sstk_hash_map_get(cycle->chain_map, post->name.data, post->name.len);

        if (chain && chain->conf) {
            // 主配置处理器
            if (chain->conf->master_handler) {
                if (chain->conf->master_handler(chain->conf, post->conf_type, post->data) != SSTK_OK) {
                    sstk_log(cycle->logger, SSTK_LOG_ERROR, "exception occured in conf [%.*s] master handler", (int) chain->name.len, chain->name.data);
                }
                continue;
            }

            // 从配置处理器
            if (chain->conf->slave_handler) {
                if (chain->conf->slave_handler(chain->conf, post->conf_type, post->data) != SSTK_OK) {
                    sstk_log(cycle->logger, SSTK_LOG_ERROR, "exception occured in conf [%.*s] slave handler", (int) chain->name.len, chain->name.data);
                }
            }
        }
    }

    // 遍历链
    sstk_queue_foreach(q, &cycle->queue) {
        chain = sstk_chain_queue_data(q);
        
        // 过期链，退出
        if (chain->expire == 0) {
            sstk_req_nonnull(chain->exit_cycle)(chain);
            sstk_remove_chain(cycle, chain);
            continue;
        }

        sstk_req_nonnull(chain->init_poll)(chain);

        sstk_req_nonnull(chain->poll)(chain);

        sstk_req_nonnull(chain->exit_poll)(chain);

        if (chain->expire > 0) {
            chain->expire--;
        }
    }
}
