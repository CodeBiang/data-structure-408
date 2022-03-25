#include <sstk_tcp_client.h>
#include <sstk_tcp_server.h>
#include <sstk_time.h>

#include "sstk_tcp_ssl_impl.h"

#if SSTK_ARCH == SSTK_ARCH_UNIX

typedef struct {
    int epfd;
    struct epoll_event ev, ev6, evs[128];
} server_ctx_t;

static sstk_conn_t* conn_add(sstk_server_t* s, socket_t sock, const remote_addr_t* addr);
static void conn_del(sstk_server_t* s, sstk_conn_t* conn);

/**
 * 服务初始化
 *
 * @param s 实体
 * @param port 端口号
 * @return errno
 */
int sstk_server_init(sstk_server_t* s) {
    struct sockaddr_in servaddr;
    struct sockaddr_in6 servaddr6;
    int flags, on;
    server_ctx_t* ctx;
    const sstk_server_opt_t* option = &s->opt;

    s->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (s->sock < 0) {
        printf("socket failed!\n");
        return SSTK_TCP_ERR_SOCK;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(option->port);

    on = 1;
    if (setsockopt(s->sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        printf("setsockopt error: %s(errno: %d)\n", strerror(errno), errno);
        return SSTK_TCP_ERR_SOCK;
    }

    on = 1;
    if (setsockopt(s->sock, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on)) == -1) {
        printf("setsockopt error: %s(errno: %d)\n", strerror(errno), errno);
        return SSTK_TCP_ERR_SOCK;
    }

    if (bind(s->sock, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1) {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        return SSTK_TCP_ERR_SOCK;
    }

    if (listen(s->sock, 1024) == -1) {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        return SSTK_TCP_ERR_SOCK;
    }

    flags = fcntl(s->sock, F_GETFL, 0);
    fcntl(s->sock, F_SETFL, flags | O_NONBLOCK);

    if (option->ipv6_support) {
        memset(&servaddr6, 0, sizeof(servaddr6));
        servaddr6.sin6_family = AF_INET6;
        servaddr6.sin6_port = htons(option->port);

        s->sock6 = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

        on = 1;
        if (setsockopt(s->sock6, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
            printf("setsockopt error: %s(errno: %d)\n", strerror(errno), errno);
            return SSTK_TCP_ERR_SOCK;
        }

        on = 1;
        if (setsockopt(s->sock6, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on)) == -1) {
            printf("setsockopt error: %s(errno: %d)\n", strerror(errno), errno);
            return SSTK_TCP_ERR_SOCK;
        }

        if (bind(s->sock6, (struct sockaddr*) &servaddr6, sizeof(servaddr6)) == -1) {
            printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
            return SSTK_TCP_ERR_SOCK;
        }

        if (listen(s->sock6, 1024) == -1) {
            printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
            return SSTK_TCP_ERR_SOCK;
        }

        flags = fcntl(s->sock6, F_GETFL, 0);
        fcntl(s->sock6, F_SETFL, flags | O_NONBLOCK);
    }

    ctx = malloc(sizeof(server_ctx_t));
    memset(ctx, 0, sizeof(server_ctx_t));

    // epoll 水平触发实现
    ctx->epfd = epoll_create(1);

    ctx->ev.data.fd = s->sock;
    ctx->ev.events = EPOLLIN;
    if (epoll_ctl(ctx->epfd, EPOLL_CTL_ADD, s->sock, &ctx->ev) != 0) {
        free(ctx);
        return SSTK_TCP_ERR_SOCK;
    }

    if (option->ipv6_support) {
        ctx->ev6.data.fd = s->sock6;
        ctx->ev6.events = EPOLLIN;
        if (epoll_ctl(ctx->epfd, EPOLL_CTL_ADD, s->sock6, &ctx->ev6) != 0) {
            free(ctx);
            return SSTK_TCP_ERR_SOCK;
        }
    }

    if (sstk_ssl_server_ctx_init(s) != SSTK_TCP_ERR_NONE) {
        free(ctx);
        return SSTK_TCP_ERR_SSL;
    }

    // 分配缓冲区
    if (!s->bufptr) {
        s->bufsize = s->bufsize != 0 ? s->bufsize : SSTK_TCP_DEFAULT_BUFFER_SIZE;

        // 动态申请
        s->bufptr = malloc(s->bufsize);
        s->buf_need_free = 1;
    }

    s->ctx = ctx;
    s->in_use = 1;

    return SSTK_TCP_ERR_NONE;
}

/**
 * 服务清理
 *
 * @param s 服务实体
 * @return errno
 */
int sstk_server_destroy(sstk_server_t* s) {
    if (!s->in_use) return SSTK_TCP_ERR_SOCK;
    s->in_use = 0;

    sstk_conn_t* c;

    for (c = s->conns; c; c = c->next) {
        if (c->in_use) {
            sstk_ssl_on_conn_close(s, s->sslctx, c);
            s->handler && s->handler(c, SSTK_TCP_EVT_CLOSE, NULL, 0, s);
            close(c->sock);
        }
    }

    if (s->opt.ssl_support) {
        sstk_ssl_server_ctx_cleanup(s);
    }

    if (s->opt.ipv6_support) {
        close(s->sock6);
        s->sock6 = -1;
    }
    close(s->sock);
    s->sock = -1;

    if (s->buf_need_free && s->bufptr) {
        free(s->bufptr);
        s->bufptr = NULL;
    }

    if (s->ctx) {
        free(s->ctx);
        s->ctx = NULL;
    }

    return 0;
}

/**
 * 服务轮询
 *
 * @param s 服务实体
 * @return errno
 */
int sstk_server_poll(sstk_server_t* s) {
    socklen_t socklen = sizeof(struct sockaddr_in);
    socklen_t socklen6 = sizeof(struct sockaddr_in6);
    struct sockaddr_in clientaddr;
    struct sockaddr_in6 clientaddr6;
    server_ctx_t* ctx = (server_ctx_t*) s->ctx;

    int ecnt;
    int epfd = ctx->epfd;
    struct epoll_event* evs = ctx->evs;

    if (!s->in_use) {
        sleep(1);
        return SSTK_TCP_ERR_SOCK;
    }

    ecnt = epoll_wait(epfd, evs, 128, 500);

    for (int i = 0; i < ecnt; i++) {
        if (evs[i].events & EPOLLIN) {
            if (evs[i].data.fd == s->sock) {
                // ipv4
                int cfd = accept(s->sock, (struct sockaddr*) &clientaddr, &socklen);
                if (cfd > 0) {
                    sstk_conn_t* ptr = conn_add(s, cfd, (struct sockaddr*) &clientaddr);

                    if (ptr) {
                        ctx->ev.data.ptr = ptr;
                        ctx->ev.events = EPOLLIN;
                        int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ctx->ev);
                        if (ret == -1) printf("epoll error\r\n");
                    } else {
                        close(cfd);
                    }
                }
            } else if (evs[i].data.fd == s->sock6) {
                // ipv6
                int cfd = accept(s->sock6, (struct sockaddr*) &clientaddr6, &socklen6);
                if (cfd > 0) {
                    sstk_conn_t* ptr6 = conn_add(s, cfd, (struct sockaddr*) &clientaddr6);

                    if (ptr6) {
                        ctx->ev.data.ptr = ptr6;
                        ctx->ev.events = EPOLLIN;
                        int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ctx->ev);
                        if (ret == -1) printf("epoll error\r\n");
                    } else {
                        close(cfd);
                    }
                }
            } else {
                sstk_conn_t* ptr = (sstk_conn_t*) evs[i].data.ptr;

                int ret = ptr->ssl ? sstk_ssl_recv(ptr->ssl, s->bufptr, s->bufsize, 0)
                                   : recv(ptr->sock, s->bufptr, s->bufsize, 0);

                if (ret > 0) {
                    s->handler && s->handler(ptr, SSTK_TCP_EVT_RECV, s->bufptr, (size_t) ret, s);
                } else {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ptr->sock, &evs[i]);
                    conn_del(s, ptr);
                }
            }
        }
    }

    return SSTK_TCP_ERR_NONE;
}

/**
 * 向tcp连接发送数据
 *
 * @param conn 连接实体
 * @param buf 数据缓冲区
 * @param size 长度
 * @return errno
 */
int sstk_conn_send(const sstk_conn_t* c, const u_char* buf, size_t size) {
    return c->ssl ? sstk_ssl_send(c->ssl, buf, size, MSG_WAITALL)
                  : send(c->sock, buf, size, MSG_WAITALL);
}

/**
 * 向tcp连接发送格式化字符串
 *
 * @param conn 连接实体
 * @param fmt 格式匹配串
 * @param ... ...
 * @return errno
 */
int sstk_conn_sendf(const sstk_conn_t* conn, const char* fmt, ...) {
    va_list vp;
    const sstk_conn_t* c = conn;
    int ret = 0;

    va_start(vp, fmt);

    // ssl 限制长度
    if (c->ssl) {
        char buf[1024];
        int len = vsnprintf(buf, sizeof(buf), fmt, vp);

        ret = sstk_ssl_send(c->ssl, buf, len, 0);
    } else {
        ret = vdprintf(c->sock, fmt, vp);
    }

    va_end(vp);
    return ret;
}

/**
 * 向连接批量发送数据
 *
 * @param s 服务实体
 * @param filter 连接过滤
 * @param buf 数据缓冲
 * @param size 长度
 * @return errno
 */
int sstk_server_batch_send(const sstk_server_t* s, sstk_conn_filter_fn filter, void* user_data,
                           const u_char* buf, size_t size) {
    sstk_conn_t* c;
    int ret;
    if (!s->in_use) return SSTK_TCP_ERR_SOCK;
    for (c = s->conns; c; c = c->next) {
        if (c->in_use && (!filter || filter(c, s, user_data))) {
            ret = c->ssl ? sstk_ssl_send(c->ssl, buf, size, MSG_WAITALL)
                         : send(c->sock, buf, size, MSG_WAITALL);
        }
    }
    return ret;
}

/**
 * 向连接批量发送格式化字符串
 *
 * @param s 服务实体
 * @param filter 连接过滤
 * @param fmt 格式匹配串
 * @param ... ...
 * @return errno
 */
int sstk_server_batch_sendf(const sstk_server_t* s, sstk_conn_filter_fn filter, void* user_data,
                            const char* fmt, ...) {
    va_list vp;
    sstk_conn_t* c;
    int ret = 0;

    if (!s) return 0;

    va_start(vp, fmt);

    // ssl 限制长度
    if (s->opt.ssl_support) {
        char buf[1024];
        int len = vsnprintf(buf, sizeof(buf), fmt, vp);

        for (c = s->conns; c; c = c->next) {
            if (c->in_use && (!filter || filter(c, s, user_data))) {
                sstk_ssl_send(c->ssl, buf, len, 0);
                ret++;
            }
        }
    } else {
        for (c = s->conns; c; c = c->next) {
            if (c->in_use && (!filter || filter(c, s, user_data))) {
                vdprintf(c->sock, fmt, vp);
                ret++;
            }
        }
    }

    va_end(vp);
    return ret;
}

/**
 * 断开tcp连接
 *
 * @param conn 连接实体
 * @return errno
 */
int sstk_conn_close(sstk_conn_t* conn) {
    conn_del(conn->serv, conn);
    return SSTK_TCP_ERR_NONE;
}

sstk_conn_t* conn_add(sstk_server_t* s, socket_t sock, const remote_addr_t* addr_ptr) {
    sstk_conn_t* conn = malloc(sizeof(sstk_conn_t));
    size_t addr_size;

    if (!conn) {
        return conn;
    }

    conn->is_ipv6 = addr_ptr->sa_family == AF_INET6;
    conn->serv = s;

    addr_size = conn->is_ipv6 ? sizeof(remote_addr_in6_t) : sizeof(remote_addr_in_t);
    conn->remote_addr = (remote_addr_t*) malloc(addr_size);
    if (!conn->remote_addr) {
        free(conn);
        return NULL;
    }
    memcpy(conn->remote_addr, addr_ptr, addr_size);

    conn->sock = sock;
    conn->user_data = NULL;
    conn->next = s->conns;
    conn->prev = NULL;

    // ssl accept
    if (sstk_ssl_on_conn_open(s, s->sslctx, conn) != SSTK_TCP_ERR_NONE) {
        free(conn);
        return NULL;
    }

    s->handler && s->handler(conn, SSTK_TCP_EVT_OPEN, NULL, 0, s);
    conn->in_use = 1;
    s->conns = conn;
    return conn;
}

void conn_del(sstk_server_t* s, sstk_conn_t* conn) {
    if (conn->prev) {
        conn->prev->next = conn->next;
    }

    if (s->conns == conn) {
        s->conns = conn->next;
    }

    // ssl del
    sstk_ssl_on_conn_close(s, s->sslctx, conn);

    s->handler && s->handler(conn, SSTK_TCP_EVT_CLOSE, NULL, 0, s);
    conn->in_use = false;
    close(conn->sock);

    free(conn->remote_addr);
    free(conn);
}

#endif
