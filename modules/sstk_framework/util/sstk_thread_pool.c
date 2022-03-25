#include <sstk_core.h>
#include <sstk_thread_pool.h>

#if (SSTK_THREADS)

struct sstk_thread_worker_s;

struct sstk_listener_node_s {
    sstk_future_listener_fn accpet;
    // 双向链表, 保证先添加的listener先执行
    struct sstk_listener_node_s* next;
    struct sstk_listener_node_s* prev;
};

struct sstk_future_impl {
    struct sstk_future_s base;
    void* result;

    
    struct sstk_thread_worker_s* worker;
    volatile bool done;
    pthread_mutex_t m;
    pthread_cond_t c;
    pthread_spinlock_t spin;

    struct {
        struct sstk_listener_node_s* head;
        struct sstk_listener_node_s* tail;
    } listeners;
};

struct sstk_thread_worker_s {
    void* (*run)(void* arg);
    void* arg;
    struct sstk_future_impl* future;
    struct sstk_thread_worker_s* next;
};

struct sstk_thread_pool_s {
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;

    sstk_logger_t* logger;

    struct sstk_thread_worker_s* queue_head; /* 链表结构，线程池中所有等待任务 */

    volatile bool shutdown; /* 是否销毁线程池 */
    volatile bool initialized;
    pthread_t* threads;
    int max_thread_num; /* 线程池中允许的活动线程数目 */
    int queue_size;     /* 当前等待队列的任务数目 */
    int queue_capacity; /* 线程队列容量 */
};

static void* sstk_thread_routine(void* arg);

sstk_thread_pool_t* sstk_thread_pool_create(sstk_logger_t* logger, int max_thread_num, int queue_capacity) {
    struct sstk_thread_pool_s* pool = malloc(sizeof(struct sstk_thread_pool_s));
    int i = 0;

    memset(pool, 0, sizeof(struct sstk_thread_pool_s));

    if (!pool || pool->initialized == true) {
        sstk_log(pool->logger, SSTK_LOG_DEBUG, "null pointer or double initialization\n");
        return NULL;
    }

    pthread_mutex_init(&(pool->queue_lock), NULL);
    pthread_cond_init(&(pool->queue_ready), NULL);

    pool->queue_head = NULL;

    pool->max_thread_num = max_thread_num;
    pool->queue_size = 0;
    pool->queue_capacity = queue_capacity;

    pool->shutdown = false;

    pool->threads = (pthread_t*) malloc(max_thread_num * sizeof(pthread_t));
    for (i = 0; i < max_thread_num; i++) {
        if (pthread_create(&(pool->threads[i]), NULL, sstk_thread_routine, pool) == -1) {
            sstk_thread_pool_destroy(pool);
            return NULL;
        }
    }

    pool->initialized = true;
    return pool;
}


int sstk_thread_pool_submit(struct sstk_thread_pool_s* pool, void* (*run)(void* arg), void* arg, struct sstk_future_s* future1) {
    struct sstk_thread_worker_s* newworker = (struct sstk_thread_worker_s*) malloc(sizeof(struct sstk_thread_worker_s));
    struct sstk_future_impl* future = (struct sstk_future_impl*) future1;
    newworker->run = run;
    newworker->arg = arg;
    newworker->next = NULL;
    newworker->future = future;
    if (future) {
        future->worker = newworker;
        future->done = false;
        future->result = NULL;
    }

    if (!newworker) return -1;

    pthread_mutex_lock(&(pool->queue_lock));

    // 判断限制
    if (pool->queue_size >= pool->queue_capacity) {
        free(newworker);
        if (future) {
            future->done = true;
        }
        pthread_mutex_unlock(&(pool->queue_lock));
        sstk_log(pool->logger, SSTK_LOG_DEBUG, "thread_pool queue oversize\n");
        return -1;
    }

    // 将任务加入到等待队列中
    struct sstk_thread_worker_s* member = pool->queue_head;
    if (member != NULL) {
        while (member->next != NULL) member = member->next;
        member->next = newworker;
    } else {
        pool->queue_head = newworker;
    }

    // assert(pool->queue_head != NULL);

    pool->queue_size++;

    pthread_mutex_unlock(&(pool->queue_lock));

    // 等待队列中有任务了，唤醒一个等待线程
    pthread_cond_signal(&(pool->queue_ready));
    return 0;
}

int sstk_thread_pool_destroy(struct sstk_thread_pool_s* pool) {
    int i;

    if (!pool) return -1;

    // 防止重复销毁
    if (!pool->initialized || pool->shutdown) return -2;

    pool->shutdown = true;
    // 唤醒所有等待线程，线程池要销毁了
    pthread_cond_broadcast(&(pool->queue_ready));

    // 阻塞等待线程退出，否则就会变成僵尸线程
    for (i = 0; i < pool->max_thread_num; i++)
        pthread_join(pool->threads[i], NULL);
    free(pool->threads);

    // 销毁等待队列
    struct sstk_thread_worker_s* head = NULL;

    while (pool->queue_head != NULL) {
        head = pool->queue_head;
        pool->queue_head = pool->queue_head->next;
        free(head);
    }

    // 条件变量和互斥量销毁
    pthread_cond_destroy(&(pool->queue_ready));
    pthread_mutex_destroy(&(pool->queue_lock));

    sstk_log(pool->logger, SSTK_LOG_DEBUG, "thread_pool destroy ok\n");
    return 0;
}

int sstk_thread_pool_free(struct sstk_thread_pool_s** ppool) {
    if (!*ppool) return -1;

    sstk_thread_pool_destroy(*ppool);
    free(*ppool);

    *ppool = NULL;
    return 0;
}

void* sstk_thread_routine(void* arg) {
    struct sstk_thread_pool_s* pool = (struct sstk_thread_pool_s*) arg;

    while (1) {
        if (pool->shutdown) break;
        
        pthread_mutex_lock(&(pool->queue_lock));
        // 如果等待队列为0并且不销毁线程池，则处于阻塞状态;
        // pthread_cond_wait是一个原子操作，等待前会解锁，唤醒后会加锁
        while (pool->queue_size == 0 && !pool->shutdown) {
            pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock));
        }

        // 线程池要销毁了
        if (pool->shutdown) {
            // 遇到break,continue,return等跳转语句，先解锁
            pthread_mutex_unlock(&(pool->queue_lock));
            break;
        }

        // assert(pool->queue_size != 0);
        // assert(pool->queue_head != NULL);

        // 等待队列长度减去1，并取出链表中的头元素
        pool->queue_size--;
        struct sstk_thread_worker_s* worker = pool->queue_head;
        pool->queue_head = worker->next;
        pthread_mutex_unlock(&(pool->queue_lock));

        // 调用回调函数，执行任务
        void* ret = (*(worker->run))(worker->arg);
        
        if (worker->future) {
            worker->future->result = ret;
            
            // notifiy listener by order
            struct sstk_listener_node_s* head = worker->future->listeners.head;
            struct sstk_listener_node_s* node = worker->future->listeners.tail;
            while (node != head) {
                if (node->accpet) node->accpet(ret);
                node = node->prev;
            }
            if (node) node->accpet(ret);

            worker->future->done = true;
            pthread_cond_broadcast(&(worker->future->c));
        }
        free(worker);
        worker = NULL;
    }
    // 不可达,雷达使用时注释掉下面两行
    // pthread_exit(NULL);
    return NULL;
}

sstk_future_t* sstk_future_create() {
    struct sstk_future_impl* impl = malloc(sizeof(struct sstk_future_impl));

    impl->base.wait = sstk_future_wait;
    impl->base.ready = sstk_future_ready;
    impl->base.add_listener = sstk_future_add_listener;
    impl->base.timed_wait = sstk_future_timed_wait;

    impl->done = false;
    pthread_mutex_init(&impl->m, NULL);
    pthread_cond_init(&impl->c, NULL);
    pthread_spin_init(&impl->spin, PTHREAD_PROCESS_SHARED);

    impl->listeners.head = NULL;
    impl->listeners.tail = NULL;

    return (sstk_future_t*) impl;
}

bool sstk_future_wait(struct sstk_future_s* future, void** result) {
    int ret;
    struct sstk_future_impl* impl = (struct sstk_future_impl*) future;
    if (impl->done) {
        if (result) *result = impl->result;
        return true;
    } else {
        pthread_mutex_lock(&impl->m);
        if ((ret = pthread_cond_wait(&impl->c, &impl->m)) != 0) {
            pthread_mutex_unlock(&(impl->m));
            (void) ret;
            return false;
        }
        if (impl->done) {
            if (impl->worker) {
                impl->worker->future = NULL;
            }
            if (result) *result = impl->result;
        }
        pthread_mutex_unlock(&(impl->m));
        return impl->done;
    }
}

bool sstk_future_timed_wait(struct sstk_future_s* future, struct timespec* timeout, void** result) {
    int ret;
    struct sstk_future_impl* impl = (struct sstk_future_impl*) future;
    if (impl->done) {
        if (result) *result = impl->result;
        return true;
    } else {
        pthread_mutex_lock(&impl->m);
        if ((ret = pthread_cond_timedwait(&impl->c, &impl->m, timeout)) != 0) {
            pthread_mutex_unlock(&(impl->m));
            (void) ret;
            return false;
        }
        if (impl->done) {
            if (impl->worker) {
                impl->worker->future = NULL;
            }
            if (result) *result = impl->result;
        }
        pthread_mutex_unlock(&(impl->m));
        return impl->done;
    }
}

struct sstk_future_s* sstk_future_add_listener(struct sstk_future_s* future, sstk_future_listener_fn listener) {
    struct sstk_future_impl* impl = (struct sstk_future_impl*) future;
    struct sstk_listener_node_s* node = (struct sstk_listener_node_s*) malloc(sizeof(struct sstk_listener_node_s));
    node->accpet = listener;
    node->prev = NULL;
    node->next = NULL;

    if (node == NULL) {
        perror("listener add failed, malloc failed");
        return &impl->base;
    }

    // 自旋 入链
    pthread_spin_lock(&impl->spin);

    if (!impl->listeners.tail) {
        impl->listeners.tail = node;
    }

    if (impl->listeners.head) {
        impl->listeners.head->prev = node;
    }

    // 头进 读安全
    node->next = impl->listeners.head;
    impl->listeners.head = node;

    pthread_spin_unlock(&impl->spin);

    return &impl->base;
}

bool sstk_future_ready(struct sstk_future_s* future) {
    struct sstk_future_impl* impl = (struct sstk_future_impl*) future;
    return impl->done;
}

void sstk_future_free(sstk_future_t* future) {
    struct sstk_future_impl* impl = (struct sstk_future_impl*) future;
    struct sstk_listener_node_s* node = impl->listeners.head;
    struct sstk_listener_node_s* tmp;

    sstk_future_wait(future, (void**) NULL);

    pthread_cond_broadcast(&impl->c);
    pthread_mutex_destroy(&impl->m);
    pthread_cond_destroy(&impl->c);
    pthread_spin_destroy(&impl->spin);

    // free listener node
    while (node) {
        tmp = node->next;
        free(node);
        node = tmp;
    }

    free(impl);
}

#endif
