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
