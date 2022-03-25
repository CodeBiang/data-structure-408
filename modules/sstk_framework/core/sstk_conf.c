#include <sstk_core.h>

void sstk_conf_post(sstk_cycle_t* cycle, sstk_conf_post_t* post) {
    if (!post->queue.next) {
        sstk_queue_init(&post->queue);
    }
    
    sstk_queue_insert_tail(&cycle->conf_post_queue, &post->queue);
}
