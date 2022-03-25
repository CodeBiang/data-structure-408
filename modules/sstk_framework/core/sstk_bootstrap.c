#include <sstk_core.h>


static sstk_bootstrap_t* sstk_bootstrap_with_cycle(sstk_bootstrap_t* const boot, sstk_cycle_t* cycle);
static sstk_bootstrap_t* sstk_bootstrap_with_allocator(sstk_bootstrap_t* const boot, sstk_allocator_t* allocator);

static sstk_bootstrap_t* sstk_bootstrap_add_logger(sstk_bootstrap_t* const boot, sstk_logger_t* logger);
static sstk_bootstrap_t* sstk_bootstrap_add_chain(sstk_bootstrap_t* const boot, sstk_chain_t* chain);
    
static sstk_bootstrap_t* sstk_bootstrap_post_conf(sstk_bootstrap_t* const boot, sstk_conf_post_t* conf);

static sstk_bootstrap_t* sstk_bootstrap_init_cycle(sstk_bootstrap_t* const boot);

static sstk_cycle_t* sstk_bootstrap_setup(sstk_bootstrap_t* const boot);

static sstk_bootstrap_t _sstk_bootstrap = {
    NULL,
    sstk_bootstrap_with_cycle,
    sstk_bootstrap_with_allocator,

    sstk_bootstrap_add_logger,
    sstk_bootstrap_add_chain,

    sstk_bootstrap_post_conf,

    sstk_bootstrap_init_cycle,

    sstk_bootstrap_setup
};

sstk_bootstrap_t* const sstk_bootstrap = &_sstk_bootstrap;


sstk_bootstrap_t* sstk_bootstrap_with_cycle(sstk_bootstrap_t* const boot, sstk_cycle_t* cycle) {
    boot->cycle = cycle;
    sstk_cycle = cycle;
    return boot;
}


sstk_bootstrap_t* sstk_bootstrap_with_allocator(sstk_bootstrap_t* const boot, sstk_allocator_t* allocator) {
    if (boot->cycle) {
        boot->cycle->allocator = allocator;
        return boot;
    }

    return NULL;
}


sstk_bootstrap_t* sstk_bootstrap_add_logger(sstk_bootstrap_t* const boot, sstk_logger_t* logger) {
    if (boot->cycle) {
        if (sstk_add_logger(boot->cycle, logger) != SSTK_OK) {
            sstk_log(sstk_cycle->logger, SSTK_LOG_DEBUG, "Bootstrap add logger failed");
        }
        return boot;
    }

    return NULL;
}


sstk_bootstrap_t* sstk_bootstrap_add_chain(sstk_bootstrap_t* const boot, sstk_chain_t* chain) {
    if (boot->cycle) {
        if (sstk_add_chain(boot->cycle, chain) != SSTK_OK) {
            sstk_log(sstk_cycle->logger, SSTK_LOG_DEBUG, "Bootstrap add chain failed");
        }
        return boot;
    }

    return NULL;
}


sstk_bootstrap_t* sstk_bootstrap_post_conf(sstk_bootstrap_t* const boot, sstk_conf_post_t* conf) {
    if (boot->cycle) {
        sstk_conf_post(boot->cycle, conf);
    }
    return boot;
}


sstk_bootstrap_t* sstk_bootstrap_init_cycle(sstk_bootstrap_t* const boot) {
    if (boot->cycle) {
        if (sstk_init_cycle(boot->cycle) != SSTK_OK) {
            sstk_log(sstk_cycle->logger, SSTK_LOG_DEBUG, "Bootstrap init cycle failed");
        }
        return boot;
    }

    return NULL;
}


sstk_bootstrap_t* sstk_bootstrap_init_chains(sstk_bootstrap_t* const boot) {

    return NULL;
}


sstk_cycle_t* sstk_bootstrap_setup(sstk_bootstrap_t* const boot) {
    if (boot->cycle) {
        
        (void)sstk_setup_cycle(boot->cycle);
    }

    return boot->cycle;
}

