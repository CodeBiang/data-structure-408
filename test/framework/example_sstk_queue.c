/**
 * @file example_sstk_queue.c
 *
 * @brief 反射式双向链队/链表存储结构 示例
 */
#include <sstk_core.h>

typedef struct {
    int idx;
    const char* msg;
    sstk_queue_t queue;
} my_data_t;

int main() {
    my_data_t h = {0, "h", sstk_queue_initializer(&h.queue)};
    my_data_t e = {1, "e", sstk_queue_initializer(&e.queue)};
    my_data_t l = {2, "l", sstk_queue_initializer(&l.queue)};
    my_data_t l1 = {3, "l", sstk_queue_initializer(&l1.queue)};
    my_data_t o = {4, "o", sstk_queue_initializer(&o.queue)};
    my_data_t w = {5, "w", sstk_queue_initializer(&w.queue)};
    my_data_t o1 = {6, "o", sstk_queue_initializer(&o1.queue)};
    my_data_t r = {7, "r", sstk_queue_initializer(&r.queue)};
    my_data_t l2 = {8, "l", sstk_queue_initializer(&l2.queue)};
    my_data_t d = {9, "d", sstk_queue_initializer(&d.queue)};

    sstk_queue_t qheader = sstk_queue_initializer(&qheader);
    sstk_queue_insert_tail(&qheader, &h.queue);
    sstk_queue_insert_tail(&qheader, &e.queue);
    sstk_queue_insert_tail(&qheader, &l.queue);
    sstk_queue_insert_tail(&qheader, &l1.queue);
    sstk_queue_insert_tail(&qheader, &o.queue);
    sstk_queue_insert_tail(&qheader, &w.queue);
    sstk_queue_insert_tail(&qheader, &o1.queue);
    sstk_queue_insert_tail(&qheader, &r.queue);
    sstk_queue_insert_tail(&qheader, &l2.queue);
    sstk_queue_insert_tail(&qheader, &d.queue);
    
    // 游标指针
    sstk_queue_t* cursor;
    printf("asc:\n");
    // 正序遍历
    sstk_queue_foreach(cursor, &qheader) {
        // 根据地址偏移反向获取源数据
        my_data_t* data = sstk_queue_data(cursor, my_data_t, queue);

        printf("idx: %d, msg: %s\n", data->idx, data->msg);
    }
    
    printf("desc:\n");
    // 逆序遍历
    sstk_queue_rforeach(cursor, &qheader) {
        // 根据地址偏移反向获取源数据
        my_data_t* data = sstk_queue_data(cursor, my_data_t, queue);

        printf("idx: %d, msg: %s\n", data->idx, data->msg);
    }
    

    return 0;
}
