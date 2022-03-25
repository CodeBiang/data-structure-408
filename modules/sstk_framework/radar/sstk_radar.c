#include <sstk_core.h>
#include <sstk_radar.h>


sstk_int_t sstk_radar_chain_init_chain(sstk_chain_t* chain) {
    sstk_radar_chain_t* ctx = (sstk_radar_chain_t*) chain->ctx;

    sstk_queue_init(&ctx->interceptors);
    sstk_queue_init(&ctx->interceptors);
    sstk_queue_init(&ctx->filters);

    return SSTK_OK;
}


sstk_int_t sstk_radar_chain_init_poll(sstk_chain_t* chain) {
    sstk_radar_chain_t* ctx = (sstk_radar_chain_t*) chain->ctx;
    sstk_radar_chain_t* prev_ctx;
    sstk_chain_t* cur;
    sstk_queue_t* q;

    ctx->in = NULL;
    ctx->in_cnt = 0;

    // 初始化轮询
    // 从上一个雷达节点接收数据
    for (q = chain->queue.prev;
         q != sstk_queue_sentinel(&chain->cycle->queue);
         q = sstk_queue_prev(q)) {
             
        cur = sstk_chain_queue_data(q);

        if (cur->type == SSTK_RADAR_CHAIN) {
            prev_ctx = (sstk_radar_chain_t*) cur->ctx;
            // 数据传导
            ctx->in = prev_ctx->out;
            ctx->in_cnt = prev_ctx->out_cnt;
        }
    }

    // 清理内存
    sstk_memzero(ctx->data, ctx->size * ctx->capacity);

    return SSTK_OK;
}


sstk_int_t sstk_radar_chain_poll(sstk_chain_t* chain) {
    sstk_radar_chain_t* ctx = (sstk_radar_chain_t*) chain->ctx;
    sstk_queue_t* q;
    sstk_radar_interceptor_t* interceptor;
    sstk_radar_filter_t* filter;
    sstk_uint_t cnt, i;
    sstk_bool_t intercepted;
    
    // 1. 插值器轮询, data->in
    // 无插值器的情况下, 则保留前一雷达链注入的in数据
    if (ctx->interpolator) {
        cnt = ctx->interpolator->interpolate(ctx->data, ctx->size, ctx->capacity, chain->conf, ctx->interpolator->arg);

        ctx->out_cnt = sstk_min(ctx->capacity, cnt);

        for (i = 0; i < ctx->out_cnt; i++) {
            ctx->out[i] = sstk_radar_chain_data(ctx, i);
        }
    } else if (ctx->process) {
        // 2. 核心处理, data, in->out
        ctx->process(ctx, chain->ctx);
    }

    // 3. 拦截器轮询, out->out
    // 处理完成后数据转换为out, 拦截遍历阶段
    if (!sstk_queue_empty(&ctx->interceptors)) {
        cnt = 0;

        for (i = 0; i < ctx->out_cnt; i++) {
            intercepted = false;
            
            sstk_queue_foreach(q, &ctx->interceptors) {
                
                interceptor = sstk_queue_data(q, sstk_radar_interceptor_t, queue);

                // 拦截器返回true, 丢弃数据元
                if (interceptor->intercept(ctx->in[i], i, chain->conf, interceptor->arg)) {
                    intercepted = true;
                    break;
                }
            }

            if (!intercepted) {
                ctx->out[cnt++] = ctx->out[i];
            }
        }

        ctx->out_cnt = cnt;
    }

    // 4. 过滤器轮询, out->out
    // 过滤阶段将所有out和cnt注入
    if (!sstk_queue_empty(&ctx->filters)) {
        sstk_queue_foreach(q, &ctx->filters) {

            filter = sstk_queue_data(q, sstk_radar_filter_t, queue);
            
            cnt = ctx->out_cnt;
            ctx->out_cnt = filter->filter(ctx->out, cnt, ctx->swap, ctx->capacity, chain->conf, filter->arg);
            ctx->out_cnt = sstk_min(ctx->out_cnt, ctx->capacity);

            for (i = 0; i < ctx->out_cnt; i++) {
                ctx->out[i] = ctx->swap[i];
            }
        }
    }

    return SSTK_OK;
}

