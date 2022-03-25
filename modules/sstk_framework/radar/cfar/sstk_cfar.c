#include "sstk_cfar.h"


extern sstk_int_t sstk_cfar_conf_handler(sstk_conf_t* conf, sstk_uint_t type, void* data);

static sstk_int_t cfar_process(sstk_radar_chain_t* ctx, sstk_conf_t* conf) {
    sstk_cfar_t* cfars = ctx->data;

    for (int i = 0; i < 10; i++) {
        cfars[i].r_idx = i;
        cfars[i].v_idx = 10 - i;
        cfars[i].a_idx = i;
    }

    sstk_log(sstk_cycle->logger, SSTK_LOG_INFO, "cfar process");

    // 示例, cfar的源->输出
    // cfar 是起始链, 一般cfar如果没有插值器是不会有输出的
    // 亦或后续将AD计算导入链, 则输入地址 in 则为AD地址
    for (int i = 0; i < 10; i++) {
        ctx->out[i] = &cfars[i];
    }

    ctx->out_cnt = 10;
    
    return SSTK_OK;
}

static sstk_radar_chain_t cfar_ctx = {
    .process = cfar_process
};


static sstk_conf_t cfar_conf = {
    NULL,
    sstk_radar_conf_master_handler,
    sstk_cfar_conf_handler,
    NULL
};

sstk_chain_t sstk_cfar_chain = {
    .name = sstk_string("cfar"),
    .version = 1,
    .type = SSTK_RADAR_CHAIN,
    .conf = &cfar_conf,
    .ctx = &cfar_ctx,
    .init_chain = sstk_radar_chain_init_chain,
    .init_poll = sstk_radar_chain_init_poll,
    .poll = sstk_radar_chain_poll,
    .exit_cycle = sstk_radar_chain_exit_cycle,
    .expire = -1,
};
