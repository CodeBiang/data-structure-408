# tcp tls封装使用方法

## 数据结构
参考.h即可
```c
```

## API 说明

### sstk_tcp_server.h

```c
/**
 * 服务初始化
 *
 * @param s 实体
 * @return errno
 */
int sstk_server_init(sstk_server_t* s);

/**
 * 服务清理
 *
 * @param s 服务实体
 * @return errno
 */
int sstk_server_destroy(sstk_server_t* s);

/**
 * 服务轮询
 *
 * @param s 服务实体
 * @return errno
 */
int sstk_server_poll(sstk_server_t* s);

/**
 * 向tcp连接发送数据
 *
 * @param conn 连接实体
 * @param buf 数据缓冲区
 * @param size 长度
 * @return errno
 */
int sstk_conn_send(const sstk_conn_t* conn, const u_char* buf, size_t size);

/**
 * 向tcp连接发送格式化字符串
 *
 * @param conn 连接实体
 * @param fmt 格式匹配串
 * @param ... ...
 * @return errno
 */
int sstk_conn_sendf(const sstk_conn_t* conn, const char* fmt, ...);

/**
 * 断开tcp连接
 *
 * @param conn 连接实体
 * @return errno
 */
int sstk_conn_close(sstk_conn_t* conn);

/**
 * 向连接批量发送数据
 *
 * @param s 服务实体
 * @param filter 连接过滤
 * @param user_data 业务数据(一般用于过滤)
 * @param buf 数据缓冲
 * @param size 长度
 * @return errno
 */
int sstk_server_batch_send(const sstk_server_t* s, sstk_conn_filter_fn filter, void* user_data,
                           const u_char* buf, size_t size);

/**
 * 向连接批量发送格式化字符串
 *
 * @param s 服务实体
 * @param filter 连接过滤
 * @param user_data 业务数据(一般用于过滤)
 * @param fmt 格式匹配串
 * @param ... ...
 * @return errno
 */
int sstk_server_batch_sendf(const sstk_server_t* s, sstk_conn_filter_fn filter, void* user_data,
                            const char* fmt, ...);

```

### sstk_tcp_client.h

```c

/**
 * 客户端初始化
 *
 * @param c 实体
 * @return errno
 */
int sstk_cli_init(sstk_cli_t* c);

/**
 * 客户端清理
 *
 * @param c 客户端实体
 * @return errno
 */
int sstk_cli_destroy(sstk_cli_t* c);

/**
 * 客户端执行连接
 *
 * @param c 实体
 * @return errno
 */
int sstk_cli_connect(sstk_cli_t* c);

/**
 * 客户端执行关闭
 *
 * @param c 实体
 * @return errno
 */
int sstk_cli_close(sstk_cli_t* c);

/**
 * 向服务端发送数据
 *
 * @param conn 连接实体
 * @param buf 数据缓冲区
 * @param size 长度
 * @return errno
 */
int sstk_cli_send(const sstk_cli_t* cli, const u_char* buf, size_t size);

/**
 * 向tcp连接发送格式化字符串
 *
 * @param conn 连接实体
 * @param fmt 格式匹配串
 * @param ... ...
 * @return errno
 */
int sstk_cli_sendf(const sstk_cli_t* cli, const char* fmt, ...);

/**
 * 客户端事件轮询
 *
 * @param c 客户端实体
 * @return errno
 */
int sstk_cli_poll(sstk_cli_t* c);
```

### sstk_ssl.h

全局调用，使用SSL需要全局初始化

```c
/**
 * 全局初始化ssl
 *
 * @return errno
 */
int sstk_ssl_init();

/**
 * 全局释放ssl
 *
 * @return errno
 */
int sstk_ssl_cleanup();
```

## 服务端demo

```c
#include <sstk_tcp_server.h>
#include <assert.h>
#include <signal.h>

#include "certs.h"

/* tcp server options */
static sstk_server_opt_t server_options = {
    .ipv6_support = 1,                                  // 启用ipv6
    .port = 11111,                                      // 端口
    .ssl_support = 1,                                   // 支持ssl
    .ssl_opt = { 
        .client_auth = 1,                               // 双向认证
        .ca = { CA_CERT_BUF, CA_CERT_BUF_SIZE },        // ca证书
        .cert = { SVR_CERT_BUF, SVR_CERT_BUF_SIZE },    // 证书
        .key = { SVR_KEY_BUF, SVR_KEY_BUF_SIZE },       // 私钥
        .key_secret = "123456",                         // 私钥口令
        .key_secret_size = sizeof("123456") - 1,        // 口令长度
        .session_timeout = 5,                           // ssl session 超时时间
        .version = SSTK_TLS_V13,                        // 使用TLSv1.3
        .cipher_list = "TLS13-AES128-GCM-SHA256",       // 设置加密套件
    }
};

sstk_server_t g_ssl_server = SSTK_SERVER_INITIALIZER;

/**
 * 事件轮询的回调处理器
 * 
 * @param c 当前触发事件的连接
 * @param e 事件码 `SSTK_TCP_EVT_XXXX`的宏定义
 * @param p 数据 可以为NULL
 * @param l 数据长度
 * @param s 当前server实例
 * 
 */
static int serv_handler(sstk_conn_t* c, int e, void* p, size_t l, const sstk_server_t* s) {
    char ipv6_buf[50];
    const char* ipv6;

    switch (e) {
        // 触发了套接字连接开启事件
        // 可以在这里加入每个连接对应的自定义用户业务数据, 置于`sstk_conn_t`的`user_data`字段中
        case SSTK_TCP_EVT_OPEN:
            if (c->is_ipv6) {
                ipv6 = inet_ntop(AF_INET6, &(((remote_addr_in6_t*) c->remote_addr)->sin6_addr),
                                 ipv6_buf, sizeof(ipv6_buf));
                printf("ipv6 add [%s]:%u\n", ipv6, ((remote_addr_in6_t*) c->remote_addr)->sin6_port);
            } else {
                printf("ipv4 add %s:%u\n", inet_ntoa(((remote_addr_in_t*) c->remote_addr)->sin_addr),
                       ((remote_addr_in_t*) c->remote_addr)->sin_port);
            }
            break;

        // 触发了套接字连接关闭事件
        // 如果申请了自定义用户数据的空间，关闭事件注意需要释放
        case SSTK_TCP_EVT_CLOSE:
            if (c->is_ipv6) {
                ipv6 = inet_ntop(AF_INET6, &(((remote_addr_in6_t*) c->remote_addr)->sin6_addr),
                                 ipv6_buf, sizeof(ipv6_buf));
                printf("ipv6 del [%s]:%u\n", ipv6, ((remote_addr_in6_t*) c->remote_addr)->sin6_port);
            } else {
                printf("ipv4 del %s:%u\n", inet_ntoa(((remote_addr_in_t*) c->remote_addr)->sin_addr),
                       ((remote_addr_in_t*) c->remote_addr)->sin_port);
            }

            break;

        // 触发了数据接收事件
        case SSTK_TCP_EVT_RECV:
            if (c->is_ipv6) {
                ipv6 = inet_ntop(AF_INET6, &(((remote_addr_in6_t*) c->remote_addr)->sin6_addr),
                                 ipv6_buf, sizeof(ipv6_buf));
                printf("ipv6 recv from [%s]:%u %d\n", 
                       ipv6,
                       ((remote_addr_in6_t*) c->remote_addr)->sin6_port, 
                       (int) l);
            } else {
                printf("ipv4 recv from %s:%u %d\n",
                       inet_ntoa(((remote_addr_in_t*) c->remote_addr)->sin_addr),
                       ((remote_addr_in_t*) c->remote_addr)->sin_port, 
                       (int) l);
                       
            }
            // 回复
            // sstk_conn_send(c, p, l);
            sstk_conn_sendf(c, "hhhh %d", 66666);
            break;

        // 触发了 TLS 连接开启事件，如果开启了TLS功能，此事件才是连接完全建立的事件，因为在时序上 OPEN->SSL_OPEN->SSL_CLOSE->CLOSE
        case SSTK_TCP_EVT_SSL_OPEN:
            printf("ssl open\n");
            break;

        // 触发了 TLS 连接关闭事件
        case SSTK_TCP_EVT_SSL_CLOSE:
            printf("ssl close\n");
            break;
    }
    return 0;
}


volatile unsigned app_shutdown = 0;

void sigint_handle(int sig) {
    app_shutdown = 1;
}

void main() {
    int ret;

    signal(SIGINT, sigint_handle);
    
    memcpy(&g_ssl_server.opt, &server_options, sizeof(sstk_server_opt_t));
    
    // 设置服务器的处理器
    g_ssl_server.handler = serv_handler;

    // 全局ssl初始化
    ret = sstk_ssl_init();
    assert(ret == SSTK_TCP_ERR_NONE);

    // tcp server 实例化
    ret = sstk_server_init(&g_ssl_server);
    assert(ret == SSTK_TCP_ERR_NONE);

    while (!app_shutdown) {
        // 事件轮询
        sstk_server_poll(&g_ssl_server);
    }

    // tcp server 销毁
    ret = sstk_server_destroy(&g_ssl_server);
    assert(ret == SSTK_TCP_ERR_NONE);
    
    // ssl 清理
    ret = sstk_ssl_cleanup();
    assert(ret == SSTK_TCP_ERR_NONE);

    printf("done\n");
}

```

## 客户端demo

```c
#include <assert.h>
#include <signal.h>
#include <sstk_tcp_client.h>

#include "certs.h"

sstk_cli_t g_ssl_cli;

static int cli_handler(sstk_cli_t* c, int e, void* p, size_t l) {
    char ipv6_buf[50];
    const char* ipv6;

    switch (e) {
        case SSTK_TCP_EVT_OPEN:
            break;
        case SSTK_TCP_EVT_CLOSE:

            break;
        case SSTK_TCP_EVT_RECV:
            printf("recv %.*s\n", (int) l, (char*) p);
            break;
        case SSTK_TCP_EVT_SSL_OPEN:
            printf("ssl open\n");
            sstk_cli_sendf(c, "hhhh %d", 66666);
            break;
        case SSTK_TCP_EVT_SSL_CLOSE:
            printf("ssl close\n");
            break;
    }
    return 0;
}

volatile unsigned app_shutdown = 0;

void sigint_handle(int sig) { app_shutdown = 1; }

void main() {
    int ret;

    static remote_addr_in_t server_addr;

    sstk_memzero(&server_addr, sizeof(server_addr));

    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(11111);

    /* tcp client options */
    sstk_cli_opt_t cli_options = {
        .auto_reconnect = 1,                         // 自动重连
        .reconnect_interval = 5000,                  // 5s重连一次
        .serv_addr = (remote_addr_t*) &server_addr,  // 服务器地址
        .ipv6_enable = 0,                            // 启用ipv6
        .ssl_enable = 1,                             // 支持ssl
        .ssl_opt = {
            .client_auth = 1,                           // 双向认证
            .ca = {CA_CERT_BUF, CA_CERT_BUF_SIZE},      // ca证书
            .cert = {CLI_CERT_BUF, CLI_CERT_BUF_SIZE},  // 证书
            .key = {CLI_KEY_BUF, CLI_KEY_BUF_SIZE},     // 私钥
            .key_secret = "123456",                     // 私钥口令
            .key_secret_size = sizeof("123456") - 1,    // 口令长度
            .session_timeout = 5,                       // ssl session 超时时间
            .version = SSTK_TLS_V13,                    // 使用TLSv1.3
            .cipher_list = "TLS13-AES128-GCM-SHA256",   // 设置加密套件
        }
    };

    signal(SIGINT, sigint_handle);

    memcpy(&g_ssl_cli.opt, &cli_options, sizeof(sstk_cli_opt_t));

    // 设置服务器的处理器
    g_ssl_cli.handler = cli_handler;

    // 全局ssl初始化
    ret = sstk_ssl_init();
    assert(ret == SSTK_TCP_ERR_NONE);

    // tcp client 实例化
    ret = sstk_cli_init(&g_ssl_cli);
    assert(ret == SSTK_TCP_ERR_NONE);

    while (!app_shutdown) {
        // 事件轮询
        sstk_cli_poll(&g_ssl_cli);
    }

    // tcp client 销毁
    printf("destroy\n");
    ret = sstk_cli_destroy(&g_ssl_cli);
    assert(ret == SSTK_TCP_ERR_NONE);

    // ssl 清理
    printf("cleanup\n");
    ret = sstk_ssl_cleanup();
    assert(ret == SSTK_TCP_ERR_NONE);

    printf("done\n");
}

```

