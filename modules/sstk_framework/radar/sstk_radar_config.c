#include <sstk_core.h>
#include <sstk_radar.h>

#define sstk_radar_handler_op(ctx, field, data, add_or_rm)   \
    if (add_or_rm) {                                         \
        sstk_queue_insert_tail(&ctx->field, &(data)->queue); \
    } else {                                                 \
        sstk_queue_remove(&(data)->queue);                   \
    }

sstk_int_t sstk_radar_conf_master_handler(sstk_conf_t* conf, sstk_uint_t type,
                                          void* data) {
    // 雷达链上下文
    sstk_radar_chain_t* ctx = (sstk_radar_chain_t*) conf->chain->ctx;

    // 配置解析
    switch (type) {
        case SSTK_RADAR_SET_INTERPOLATOR_CONF:
            ctx->interpolator = (sstk_radar_interpolator_t*) data;
            return SSTK_OK;
        case SSTK_RADAR_RM_INTERPOLATOR_CONF:
            ctx->interpolator = NULL;
            return SSTK_OK;
        case SSTK_RADAR_ADD_INTERCEPTOR_CONF:
            sstk_radar_handler_op(ctx, interceptors,
                                  (sstk_radar_interceptor_t*) data, 1);
            return SSTK_OK;
        case SSTK_RADAR_RM_INTERCEPTOR_CONF:
            sstk_radar_handler_op(ctx, interceptors,
                                  (sstk_radar_interceptor_t*) data, 0);
            return SSTK_OK;
        case SSTK_RADAR_ADD_FILTER_CONF:
            sstk_radar_handler_op(ctx, filters, (sstk_radar_filter_t*) data, 1);
            return SSTK_OK;
        case SSTK_RADAR_RM_FILTER_CONF:
            sstk_radar_handler_op(ctx, filters, (sstk_radar_filter_t*) data, 0);
            return SSTK_OK;
        default:
            return SSTK_ERR;
    }
}

sstk_int_t sstk_radar_chain_exit_cycle(sstk_chain_t* radar_chain) {
    // 释放内存
    sstk_radar_chain_t* ctx = (sstk_radar_chain_t*) radar_chain->ctx;

    if (ctx->out) {
        radar_chain->cycle->allocator->dealloc_fn(ctx->out);
        ctx->out = NULL;
    }

    if (ctx->swap) {
        radar_chain->cycle->allocator->dealloc_fn(ctx->swap);
        ctx->swap = NULL;
    }

    return SSTK_OK;
}
