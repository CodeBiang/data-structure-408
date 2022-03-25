#include <sstk_tcp_client.h>
#include <sstk_time.h>

#include "sstk_tcp_ssl_impl.h"

/**
 * 客户端初始化
 *
 * @param c 实体
 * @param port 端口号
 * @return errno
 */
int sstk_cli_init(sstk_cli_t* c) {
    sstk_cli_opt_t* opt = &c->opt;

    if (sstk_ssl_cli_ctx_init(c) != SSTK_TCP_ERR_NONE) {
        return SSTK_TCP_ERR_SSL;
    }

    // 分配缓冲区
    if (!c->bufptr) {
        c->bufsize = c->bufsize != 0 ? c->bufsize : SSTK_TCP_DEFAULT_BUFFER_SIZE;

        // 动态申请
        c->bufptr = malloc(c->bufsize);
        c->buf_need_free = 1;
    }

    c->connected = false;
    c->in_use = true;

    return SSTK_TCP_ERR_NONE;
}

/**
 * 客户端清理
 *
 * @param c 客户端实体
 * @return errno
 */
int sstk_cli_destroy(sstk_cli_t* c) {
    if (!c->in_use) return SSTK_TCP_ERR_SOCK;

    sstk_cli_close(c);

    if (c->opt.ssl_enable) {
        sstk_ssl_cli_ctx_cleanup(c);
    }

    if (c->buf_need_free && c->bufptr) {
        free(c->bufptr);
        c->bufptr = NULL;
    }

    c->in_use = false;

    return SSTK_TCP_ERR_NONE;
}

/**
 * 客户端执行连接
 *
 * @param c 实体
 * @return errno
 */
int sstk_cli_connect(sstk_cli_t* c) {
    if (!c->in_use) return SSTK_TCP_ERR_ABORT;

    if (c->connected) {
        // 关闭
        sstk_cli_close(c);
    }

    // 重创建套接字
    c->sock = socket(c->opt.ipv6_enable ? AF_INET6 : AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (c->sock < 0) {
        return SSTK_TCP_ERR_SOCK;
    }
    perror("BEFORE");
    if (connect(c->sock, (struct sockaddr*) c->opt.serv_addr,
                c->opt.ipv6_enable ? sizeof(remote_addr_in6_t) : sizeof(remote_addr_in_t))
        < 0) {
        perror("connect");
        return SSTK_TCP_ERR_SOCK;
    }

    c->handler && c->handler(c, SSTK_TCP_EVT_OPEN, NULL, 0);

    if (sstk_ssl_on_cli_open(c) != SSTK_TCP_ERR_NONE) {
        perror("ssl connect");

        // 关闭
        sstk_cli_close(c);
        return SSTK_TCP_ERR_SOCK;
    }

    c->connected = true;

    return SSTK_TCP_ERR_NONE;
}

/**
 * 客户端执行关闭
 *
 * @param c 实体
 * @return errno
 */
int sstk_cli_close(sstk_cli_t* c) {
    if (c->connected) return SSTK_TCP_ERR_NONE;

    // ssl del
    sstk_ssl_on_cli_close(c);

    c->handler && c->handler(c, SSTK_TCP_EVT_CLOSE, NULL, 0);
    c->connected = false;
    close(c->sock);
    c->sock = -1;

    free(c);

    if (c->connected) return SSTK_TCP_ERR_NONE;
}

/**
 * 向服务端发送数据
 *
 * @param conn 连接实体
 * @param buf 数据缓冲区
 * @param size 长度
 * @return errno
 */
int sstk_cli_send(const sstk_cli_t* c, const u_char* buf, size_t size) {
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
int sstk_cli_sendf(const sstk_cli_t* cli, const char* fmt, ...) {
    va_list vp;
    const sstk_cli_t* c = cli;
    int ret = 0;
    char buf[1024];
    int len;

    va_start(vp, fmt);

    // ssl 限制长度
    if (c->ssl) {
        len = vsnprintf(buf, sizeof(buf), fmt, vp);

        ret = sstk_ssl_send(c->ssl, buf, len, 0);
    } else {
#if SSTK_ARCH == SSTK_ARCH_UNIX
        ret = vdprintf(c->sock, fmt, vp);
#else
        len = vsnprintf(buf, sizeof(buf), fmt, vp);
        ret = send(c->ssl, buf, len, 0);
#endif
    }

    va_end(vp);
    return ret;
}

/**
 * 客户端事件轮询
 *
 * @param c 客户端实体
 * @return errno
 */
int sstk_cli_poll(sstk_cli_t* c) {
    fd_set set;
    struct timeval tv;
    int ret;

    if (!c->in_use) return SSTK_TCP_ERR_ABORT;

    if (!c->connected) {
        // 执行重连
        if (c->opt.auto_reconnect) {
            if (sstk_cli_connect(c) != SSTK_TCP_ERR_NONE) {
                // poll 内 延时
                sstk_msleep(c->opt.reconnect_interval);
                printf("重连失败 延时等待\n");
                return SSTK_TCP_ERR_NONE;
            }
        } else
            return SSTK_TCP_ERR_ABORT;
    }

    FD_ZERO(&set);
    FD_SET(c->sock, &set);

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    switch (select(c->sock + 1, &set, NULL, NULL, &tv)) {
        case -1:
            // error occured
            printf("error occured!\n");
            break;
        case 0:
            // timeout
            printf("timeout\n");
            break;
        default:
            if (FD_ISSET(c->sock, &set)) {
                ret = c->ssl ? sstk_ssl_recv(c->ssl, c->bufptr, c->bufsize, 0)
                             : recv(c->sock, c->bufptr, c->bufsize, 0);

                if (ret <= 0) {
                    sstk_cli_close(c);
                } else {
                    c->handler && c->handler(c, SSTK_TCP_EVT_RECV, c->bufptr, (size_t) ret);
                }
            }
    }
}