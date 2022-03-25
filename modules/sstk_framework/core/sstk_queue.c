#include <sstk_core.h>

void sstk_queue_sort(sstk_queue_t* queue,
                     sstk_int_t (*cmp)(const sstk_queue_t*,
                                       const sstk_queue_t*)) {
    sstk_queue_t *q, *prev, *next;

    q = sstk_queue_head(queue);

    if (q == sstk_queue_last(queue)) {
        return;
    }

    for (q = sstk_queue_next(q); q != sstk_queue_sentinel(queue); q = next) {
        prev = sstk_queue_prev(q);
        next = sstk_queue_next(q);

        sstk_queue_remove(q);

        do {
            if (cmp(prev, q) <= 0) {
                break;
            }

            prev = sstk_queue_prev(prev);

        } while (prev != sstk_queue_sentinel(queue));

        sstk_queue_insert_after(prev, q);
    }
}
