#include <sstk_tcp_server.h>

#include "sstk_tcp_ssl_impl.h"

// #define UNIX_DEBUG

#ifdef UNIX_DEBUG
#define SSTK_ARCH SSTK_ARCH_TI_OS
#include <arpa/inet.h>
#include <sys/socket.h>

#define PSA struct sockaddr*
#define INVALID_SOCKET -1
#endif

#if SSTK_ARCH == SSTK_ARCH_TI_OS

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
    struct timeval timeout;
    int on = 1;
    const sstk_server_opt_t* option = &s->opt;

    fdOpenSession(TaskSelf());

    s->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s->sock == INVALID_SOCKET) {
        // Dbg_printf(" NETWORK: Unable to open socket (port=%d)!!!\n", port);
        return SSTK_TCP_ERR_SOCK;
    }

    // bzero(&servaddr, sizeof(servaddr));
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(option->port);

    setsockopt(s->sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    if (bind(s->sock, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1) {
        // Dbg_printf(" NETWORK: Unable to bind() (port=%d) !!!\n", port);
        close(s->sock);
        s->sock = INVALID_SOCKET;
        // sstk_server_destroy(s);
        return SSTK_TCP_ERR_SOCK;
    }

    if (listen(s->sock, 5) == -1) {
        // Dbg_printf(" NETWORK: Listen() is fail (port=%d) !!!\n", port);
        close(s->sock);
        s->sock = INVALID_SOCKET;
        // sstk_server_destroy(s);
        return SSTK_TCP_ERR_SOCK;
    }

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    setsockopt(s->sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    setsockopt(s->sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    if (option->ipv6_support) {
        memset(&servaddr6, 0, sizeof(servaddr6));
        servaddr6.sin6_family = AF_INET6;
        servaddr6.sin6_port = htons(option->port);

        s->sock6 = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

        on = 1;
        setsockopt(s->sock6, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        // 同时监听ipv4和ipv6
        on = 1;
        setsockopt(s->sock6, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on));

        if (bind(s->sock6, (struct sockaddr*) &servaddr6, sizeof(servaddr6)) == -1) {
            return SSTK_TCP_ERR_SOCK;
        }

        if (listen(s->sock6, 1024) == -1) return SSTK_TCP_ERR_SOCK;
    }

    if (sstk_ssl_server_ctx_init(s) != SSTK_TCP_ERR_NONE) {
        return SSTK_TCP_ERR_SSL;
    }

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
    if (!s->in_use || s->sock == INVALID_SOCKET) return SSTK_TCP_ERR_SOCK;
    s->in_use = 0;

    sstk_conn_t* c;

    for (c = s->conns; c; c = c->next) {
        if (c->in_use && c->sock != INVALID_SOCKET) {
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
    }
    close(s->sock);
    s->sock = INVALID_SOCKET;

    if (s->opt.ipv6_support) {
        close(s->sock6);
        s->sock6 = INVALID_SOCKET;
    }

    TaskBlock(TaskSelf());
    return SSTK_TCP_ERR_NONE;
}

/**
 * 服务轮询
 *
 * @param s 服务实体
 * @return errno
 */
int sstk_server_poll(sstk_server_t* s) {
    fd_set ibits;
    // 无复制 内存
    char pbuf[1024];
    sstk_conn_t* c;
    socket_t client;
    struct sockaddr_in clientaddr;
    struct sockaddr_in6 clientaddr6;
    struct timeval tv;
    int max_fd;
    int size;
    int cnt;

    if (!s->in_use || s->sock == INVALID_SOCKET) {
        // Dbg_printf("error! in_use:%d sock:0x%x\n",s->in_use,s->sock);
        return SSTK_TCP_ERR_SOCK;
    }

    FD_ZERO(&ibits);
    FD_SET(s->sock, &ibits);
    max_fd = (int) s->sock;

    // ipv6
    if (s->opt.ipv6_support) {
        FD_SET(s->sock6, &ibits);
        max_fd = (int) s->sock6 > max_fd ? (int) s->sock6 : max_fd;
    }

    for (c = s->conns; c; c = c->next) {
        if (c->in_use && c->sock != INVALID_SOCKET) {
            FD_SET(c->sock, &ibits);
            max_fd = (int) c->sock > max_fd ? (int) c->sock : max_fd;
        }
    }

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    switch (select(max_fd + 1, &ibits, NULL, NULL, &tv)) {
        case -1:
            // error occured
            // Dbg_printf("error occured!\n");
            break;
        case 0:
            // timeout
            // Dbg_printf("timeout\n");
            break;
        default:
            if (FD_ISSET(s->sock, &ibits)) {
                // Dbg_printf("get linking!\n");
                size = sizeof(clientaddr);
                client = accept(s->sock, (PSA) &clientaddr, &size);
                (client != INVALID_SOCKET) && conn_add(s, client, (remote_addr_t*) &clientaddr);
            } else if (FD_ISSET(s->sock6, &ibits)) {
                // ipv6的连接
                size = sizeof(clientaddr6);
                client = accept(s->sock6, (PSA) &clientaddr6, &size);
                (client != INVALID_SOCKET) && conn_add(s, client, (remote_addr_t*) &clientaddr6);
            } else {
                for (c = s->conns; c; c = c->next) {
                    if (c->in_use && c->sock != INVALID_SOCKET && FD_ISSET(c->sock, &ibits)) {
                        cnt = (int) c->ssl ? sstk_ssl_recv(c->ssl, pbuf, sizeof(pbuf), 0)
                                           : recv(c->sock, pbuf, sizeof(pbuf), 0);
                        // Dbg_printf("recv datas %d!\n",cnt);

                        if (cnt <= 0) {
                            conn_del(s, c);
                        } else {
                            // Dbg_printf("recv:%.*s\n",cnt,pbuf);
                            // send(c->sock, pbuf, cnt, 0);
                            s->handler && s->handler(c, SSTK_TCP_EVT_RECV, NULL, (size_t) cnt, s);
                        }
                    }
                }
            }
    }
    return 0;
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
    char buf[1024];
    int len;

    if (!s) return 0;

    va_start(vp, fmt);

    // ssl 限制长度
    len = vsnprintf(buf, sizeof(buf), fmt, vp);
    for (c = s->conns; c; c = c->next) {
        if (c->in_use && (!filter || filter(c, s, user_data))) {
            s->opt.ssl_support ? sstk_ssl_send(c->ssl, buf, len, 0) : send(c->sock, buf, len, 0);
            ret++;
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

static void conn_del(sstk_server_t* s, sstk_conn_t* conn) {
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

/**
 * @description: 手动实现inet_ntoa
 * @param {in_addr} in ：ip地址
 * @return {char *} str：字符串形式ip地址
 * @author: 21135
 */
char* inet_ntoa(struct in_addr in) {
    static char str[16] = {0};
    uint32_t ip_addr = in.s_addr;

    memset(str, 0, 16);
    snprintf(str, 16, "%d.%d.%d.%d", (uint8_t) ((ip_addr >> 24) & 0xFF),
             (uint8_t) ((ip_addr >> 16) & 0xFF), (uint8_t) ((ip_addr >> 8) & 0xFF),
             (uint8_t) (ip_addr & 0xFF));

    return str;
}

#endif
