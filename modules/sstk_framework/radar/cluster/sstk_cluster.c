#include "sstk_cluster.h"

#include <cfar/sstk_cfar.h>

extern sstk_uint_t sstk_cluster_process(sstk_cfar_t** in, sstk_uint_t in_cnt,
                                        sstk_cluster_t* clusters,
                                        sstk_uint_t capacity,
                                        sstk_cluster_t** out, sstk_conf_t* conf,
                                        const sstk_radar_conf_ctx_t* rconf);

static int cfar_cmp(const void* c1, const void* c2) {
    const void* p1 = *((void**) c1);
    const void* p2 = *((void**) c2);

    return ((sstk_cfar_t*) p1)->r_idx > ((sstk_cfar_t*) p2)->r_idx ? 1 : -1;
}

static sstk_int_t cluster_process(sstk_radar_chain_t* ctx, sstk_conf_t* conf) {
    sstk_cfar_t** cfars = (sstk_cfar_t**) ctx->in;
    // 1. 从全局链拿到
    sstk_radar_conf_ctx_t* rconf = (sstk_radar_conf_ctx_t*) sstk_radar_chain.conf->ctx;
    // 2. 或者从生命周期管理器中搜索拿到链
    // sstk_chain_t* rchain = (sstk_chain_t*)
    // sstk_hash_map_get(sstk_cycle->chain_map, (unsigned char*) "radar",
    // sizeof("radar") - 1); sstk_radar_conf_ctx_t* rconf =
    // (sstk_radar_conf_ctx_t*) (rchain)->conf->ctx;

    // 点排序
    sstk_qsort(cfars, ctx->in_cnt, sizeof(void*), cfar_cmp);

    // 核心数据处理
    ctx->out_cnt = sstk_cluster_process(cfars, ctx->in_cnt, 
                                        (sstk_cluster_t*) ctx->data, 
                                        ctx->capacity,
                                        (sstk_cluster_t**) ctx->out, 
                                        conf, rconf);

    sstk_log(sstk_cycle->logger, SSTK_LOG_WARN,
             "cluster process, get input data count %d, process out data count %d", 
             (int) ctx->in_cnt, (int) ctx->out_cnt);

    sstk_log(sstk_cycle->logger, SSTK_LOG_INFO, "get rconf wavelength %f",
             rconf->spec->wavelength);

    return SSTK_OK;
}

sstk_radar_chain_t sstk_cluster_chain_ctx = {.process = cluster_process};

sstk_chain_t sstk_cluster_chain = {
    .name = sstk_string("cluster"),
    .version = 1,
    .type = SSTK_RADAR_CHAIN,
    .ctx = &sstk_cluster_chain_ctx,
    .init_chain = sstk_radar_chain_init_chain,
    .init_poll = sstk_radar_chain_init_poll,
    .poll = sstk_radar_chain_poll,
    .exit_cycle = sstk_radar_chain_exit_cycle,
    .expire = -1,
};
