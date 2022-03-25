#include <sstk_core.h>
#include <sstk_math.h>
#include <sstk_radar.h>

static sstk_int_t radar_conf_handler(sstk_conf_t* conf, sstk_radar_conf_t* data);
static sstk_int_t sstk_radar_core_conf_master_handler(sstk_conf_t* conf, sstk_uint_t type, void* data);

sstk_radar_conf_t sstk_radar_conf = {0};
sstk_radar_spec_t sstk_radar_spec = {0};

static sstk_core_chain_t sstk_radar_chain_ctx = {
    sstk_string("radar"),
    NULL
};

static sstk_radar_conf_ctx_t radar_conf_ctx = {
    &sstk_radar_conf,
    &sstk_radar_spec
};

static sstk_conf_t radar_conf = {
    &radar_conf_ctx,
    sstk_radar_core_conf_master_handler,
    NULL,
    NULL
};

sstk_chain_t sstk_radar_chain = {
    sstk_string("radar"),
    1,
    SSTK_CORE_CHAIN,
    &sstk_radar_chain_ctx,
    &radar_conf,
    -1,
    NULL,
    NULL,
    NULL
};

sstk_int_t sstk_radar_core_conf_master_handler(sstk_conf_t* conf, sstk_uint_t type, void* data) {
    switch (type) {
        case SSTK_RADAR_CONF:
            return radar_conf_handler(conf, (sstk_radar_conf_t*) data);
        default:
            return SSTK_ERR;
    }
}

sstk_int_t radar_conf_handler(sstk_conf_t* conf, sstk_radar_conf_t* data) {
    sstk_radar_conf_ctx_t* ctx = (sstk_radar_conf_ctx_t*) conf->ctx;
    sstk_logger_t* logger = conf->chain->cycle->logger;

    // 配置
    sstk_memcpy(ctx->conf, data, sizeof(sstk_radar_conf_t));


    // 波长
    ctx->spec->wavelength = SSTK_RADAR_LIGHT_SPEED / ctx->conf->freq;
    // 默认天线间距 lamda / 2
    ctx->conf->antenna_distance = ctx->conf->antenna_distance == 0 ? (ctx->spec->wavelength / 2) : ctx->conf->antenna_distance;
    
    // 计算距离分辨率
    ctx->spec->r_res = SSTK_RADAR_LIGHT_SPEED / 2 / ctx->conf->bandwidth;
    // 计算速度分辨率
    ctx->spec->v_res = ctx->spec->wavelength / 2 / ctx->conf->prt / ctx->conf->fft2d / ctx->conf->tx_cnt;
    // 计算角度分辨率
    ctx->spec->a_res = ctx->spec->wavelength / (ctx->conf->rx_cnt * ctx->conf->antenna_distance); // lamda / (N * d * cos(theta))

    // 最大探测距离
    ctx->spec->r_max = ctx->spec->r_res * ctx->conf->fft1d;
    // 最大速度, 正负等分
    ctx->spec->v_max = ctx->spec->v_res * ctx->conf->fft2d * ctx->conf->tx_cnt;
    // 最大角度
    ctx->spec->a_max = sstk_asinf(ctx->spec->wavelength / (2 * ctx->conf->antenna_distance));

    sstk_log(logger, SSTK_LOG_INFO, "\nradar configured\n"
             " -> wavelength: %f\n"
             " -> r_res: %f\n"
             " -> v_res: %f\n"
             " -> a_res: %f\n"
             " -> r_max: %f\n"
             " -> v_max: %f\n"
             " -> a_max: %f\n",
             ctx->spec->wavelength,
             ctx->spec->r_res,
             ctx->spec->v_res,
             ctx->spec->a_res,
             ctx->spec->r_max,
             ctx->spec->v_max,
             ctx->spec->a_max);

    return SSTK_OK;
}
