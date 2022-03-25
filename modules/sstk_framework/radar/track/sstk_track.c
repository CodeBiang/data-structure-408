#include "sstk_track.h"

static sstk_int_t track_process(sstk_radar_chain_t* ctx, sstk_conf_t* conf) {
    sstk_log(sstk_cycle->logger, SSTK_LOG_INFO, "track process");
    return SSTK_OK;
}


sstk_radar_chain_t sstk_track_chain_ctx = {
    .process = track_process
};

sstk_chain_t sstk_track_chain = {
    .name = sstk_string("track"),
    .version = 1,
    .type = SSTK_RADAR_CHAIN,
    .ctx = &sstk_track_chain_ctx,
    .init_chain = sstk_radar_chain_init_chain,
    .init_poll = sstk_radar_chain_init_poll,
    .poll = sstk_radar_chain_poll,
    .exit_cycle = sstk_radar_chain_exit_cycle,
    .expire = -1,
};
