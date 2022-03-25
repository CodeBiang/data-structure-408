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

static int serv_handler(sstk_conn_t* c, int e, void* p, size_t l, const sstk_server_t* s) {
    char ipv6_buf[50];
    const char* ipv6;

    switch (e) {
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
        case SSTK_TCP_EVT_SSL_OPEN:
            printf("ssl open\n");
            break;
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
