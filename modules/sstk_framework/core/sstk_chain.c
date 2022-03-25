#include <sstk_core.h>

sstk_int_t sstk_init_chains(sstk_cycle_t* cycle) {
    sstk_queue_t* q;
    sstk_chain_t* c;

    sstk_queue_foreach(q, &cycle->queue) {
        c = sstk_chain_queue_data(q);
        
        if (c->init_chain) {
            c->init_chain(c);
        }
    }

    return SSTK_ERR;
}

sstk_int_t sstk_add_chain(sstk_cycle_t* cycle, sstk_chain_t* chain) {
    sstk_int_t ret;

    if (!chain->queue.next) {
        sstk_queue_init(&chain->queue);
    }

    if (!cycle->chain_map) return SSTK_ERR;

    // 不允许重复名称
    if (sstk_hash_map_get(cycle->chain_map, chain->name.data, chain->name.len)) {
        return SSTK_ABORT;
    }

    ret = sstk_hash_map_put(cycle->chain_map, chain->name.data, chain->name.len, chain);

    if (ret != SSTK_OK) {
        return ret;
    }

    // 双向绑定
    chain->cycle = cycle;
    if (chain->conf) {
        chain->conf->chain = chain;
    }

    // 添加至队列
    sstk_queue_insert_tail(&cycle->queue, &chain->queue);

    // 添加至cycle 回调执行
    if (chain->init_cycle) {
        chain->init_cycle(chain);
    }

    sstk_log(cycle->logger, SSTK_LOG_DEBUG, "chain %.*s added", (int)chain->name.len,
            (char*)chain->name.data);

    return SSTK_OK;
}

sstk_chain_t* sstk_remove_chain_by_name(sstk_cycle_t* cycle, sstk_str_t name) {
    sstk_chain_t* chain = NULL;

    if (!cycle->chain_map) return chain;

    chain = sstk_hash_map_remove(cycle->chain_map, name.data, name.len);

    if (chain) {
        sstk_remove_chain(cycle, chain);
    }

    return chain;
}

sstk_int_t sstk_remove_chain(sstk_cycle_t* cycle, sstk_chain_t* chain) {
    sstk_queue_remove(&chain->queue);
    sstk_log(cycle->logger, SSTK_LOG_DEBUG, "chain %.*s removed", (int)chain->name.len,
            (char*)chain->name.data);
    return SSTK_OK;
}
