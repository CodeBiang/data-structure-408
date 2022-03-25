#include "sstk_tcp_ssl_impl.h"

#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/wolfcrypt/random.h>
#include <wolfssl/wolfcrypt/types.h>

static unsigned ssl_initialized = 0;

int sstk_ssl_on_conn_open(sstk_server_t* serv, void* sctx, sstk_conn_t* conn) {
    if (!ssl_initialized || !sctx) return SSTK_SSL_ERROR_UNINITIALIZED;

    int ret;
    SSL_CTX* sslctx = (SSL_CTX*) sctx;
    SSL* ssl;
    // 新建 ssl ctx
    ssl = wolfSSL_new(sslctx);
    if (!ssl) return SSTK_SSL_ERROR_VERIFY;
    ret = wolfSSL_set_fd(ssl, conn->sock);
    if (ret != WOLFSSL_SUCCESS) goto err;

    ret = wolfSSL_accept(ssl);
    if (ret != WOLFSSL_SUCCESS) goto err;

    conn->ssl = ssl;

    serv->handler && serv->handler(conn, SSTK_TCP_EVT_SSL_OPEN, NULL, 0, serv);

    return SSTK_TCP_ERR_NONE;

err:
    // 关闭 SSL 连接
    wolfSSL_shutdown(ssl);
    // 释放 SSL
    wolfSSL_free(ssl);

    conn->ssl = NULL;

    return SSTK_SSL_ERROR_VERIFY;
}

int sstk_ssl_on_conn_close(sstk_server_t* serv, void* sctx, sstk_conn_t* conn) {
    (void) sctx;
    if (!ssl_initialized) return SSTK_SSL_ERROR_UNINITIALIZED;

    if (conn->ssl) {
        SSL* ssl = (SSL*) conn->ssl;
        // 关闭 SSL 连接
        wolfSSL_shutdown(ssl);
        // 释放 SSL
        wolfSSL_free(ssl);
        conn->ssl = NULL;

        serv->handler && serv->handler(conn, SSTK_TCP_EVT_SSL_CLOSE, NULL, 0, serv);
    }
    return SSTK_TCP_ERR_NONE;
}

int sstk_ssl_send(void* cssl, const void* data, size_t size, int flags) {
    SSL* ssl = (SSL*) cssl;
    return wolfSSL_send(ssl, data, (int) size, flags);
}

int sstk_ssl_recv(void* cssl, void* data, size_t size, int flags) {
    SSL* ssl = (SSL*) cssl;
    return wolfSSL_recv(ssl, data, (int) size, flags);
}

int sstk_ssl_init() {
    if (!ssl_initialized) {
        if (wolfSSL_Init() == WOLFSSL_SUCCESS) {
            ssl_initialized = 1;
            return SSTK_TCP_ERR_NONE;
        } else
            return SSTK_TCP_ERR_ABORT;
    }
    return SSTK_TCP_ERR_NONE;
}

int sstk_ssl_cleanup() {
    if (ssl_initialized) {
        if (wolfSSL_Cleanup() == WOLFSSL_SUCCESS) {
            ssl_initialized = 0;
            return SSTK_TCP_ERR_NONE;
        } else
            return SSTK_TCP_ERR_ABORT;
    }
    return SSTK_TCP_ERR_NONE;
}

static int pass_cb(char* buf, int len, int verify, void* password) {
    const sstk_ssl_opt_t* opt = (const sstk_ssl_opt_t*) password;
    snprintf(buf, len, "%.*s", opt->key_secret_size, opt->key_secret);
    printf("password: %s\n", buf);
    return strlen(buf);
}

static int ssl_ctx_init(SSL_CTX* ctx, sstk_ssl_opt_t* opt) {
    // 证书口令
    if (opt->key_secret) {
        SSL_CTX_set_default_passwd_cb(ctx, pass_cb);
        SSL_CTX_set_default_passwd_cb_userdata(ctx, opt);
    }

    // 双向认证
    if (opt->client_auth) {
        if (!opt->ca.pembuf) return SSTK_SSL_ERROR_CA;

        SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
        if (wolfSSL_CTX_load_verify_buffer(ctx, opt->ca.pembuf, opt->ca.size, SSL_FILETYPE_PEM)
            != SSL_SUCCESS)
            return SSTK_SSL_ERROR_CA;
    }

    // 加载证书
    if (wolfSSL_CTX_use_certificate_buffer(ctx, opt->cert.pembuf, opt->cert.size, SSL_FILETYPE_PEM)
        != SSL_SUCCESS)
        return SSTK_SSL_ERROR_CERT;

    // 加载key
    if (wolfSSL_CTX_use_PrivateKey_buffer(ctx, opt->key.pembuf, opt->key.size, SSL_FILETYPE_PEM)
        != SSL_SUCCESS)
        return SSTK_SSL_ERROR_KEY;

    // 检查用户私钥是否正确
    if (SSL_CTX_check_private_key(ctx) != SSL_SUCCESS) return SSTK_SSL_ERROR_KEY;

    // 设置超时
    SSL_CTX_set_timeout(ctx, opt->session_timeout);

    return SSTK_TCP_ERR_NONE;
}

int sstk_ssl_server_ctx_init(sstk_server_t* serv) {
    wolfSSL_method_func method = NULL;
    sstk_ssl_opt_t* opt = &serv->opt.ssl_opt;
    int ret;

    if (!serv->opt.ssl_support) return SSTK_OK;

    switch (opt->version) {
        case SSTK_TLS_V11:
            method = wolfTLSv1_1_server_method_ex;
            break;
        case SSTK_TLS_V12:
            method = wolfTLSv1_2_server_method_ex;
            break;
        case SSTK_TLS_V13:
            method = wolfTLSv1_3_server_method_ex;
            break;
        default:
            return SSTK_SSL_ERROR_UNKNOWN_TLS_VERSION;
    }

    SSL_CTX* ctx = NULL;

    ctx = wolfSSL_CTX_new(method(NULL));

    if (!ctx) return SSTK_TCP_ERR_ABORT;

    ret = ssl_ctx_init(ctx, opt);

    if (ret != SSTK_TCP_ERR_NONE) return ret;

    serv->sslctx = ctx;

    return SSTK_TCP_ERR_NONE;
}

int sstk_ssl_server_ctx_cleanup(sstk_server_t* serv) {
    SSL_CTX* ctx = (SSL_CTX*) serv->sslctx;
    wolfSSL_CTX_free(ctx);
    return SSTK_TCP_ERR_NONE;
}

int sstk_ssl_cli_ctx_init(sstk_cli_t* cli) {
    wolfSSL_method_func method = NULL;
    sstk_ssl_opt_t* opt = &cli->opt.ssl_opt;
    int ret;

    if (!cli->opt.ssl_enable) return SSTK_OK;

    switch (opt->version) {
        case SSTK_TLS_V11:
            method = wolfTLSv1_1_client_method_ex;
            break;
        case SSTK_TLS_V12:
            method = wolfTLSv1_2_client_method_ex;
            break;
        case SSTK_TLS_V13:
            method = wolfTLSv1_3_client_method_ex;
            break;
        default:
            return SSTK_SSL_ERROR_UNKNOWN_TLS_VERSION;
    }

    SSL_CTX* ctx = NULL;

    ctx = wolfSSL_CTX_new(method(NULL));

    if (!ctx) return SSTK_TCP_ERR_ABORT;

    ret = ssl_ctx_init(ctx, opt);

    if (ret != SSTK_TCP_ERR_NONE) return ret;

    cli->sslctx = ctx;

    return SSTK_TCP_ERR_NONE;
}

int sstk_ssl_cli_ctx_cleanup(sstk_cli_t* cli) {
    SSL_CTX* ctx = (SSL_CTX*) cli->sslctx;
    wolfSSL_CTX_free(ctx);
    return SSTK_TCP_ERR_NONE;
}

int sstk_ssl_on_cli_open(sstk_cli_t* cli) {
    if (!ssl_initialized || !cli->sslctx) return SSTK_SSL_ERROR_UNINITIALIZED;

    int ret;
    SSL_CTX* sslctx = (SSL_CTX*) cli->sslctx;
    SSL* ssl;
    // 新建 ssl ctx
    ssl = wolfSSL_new(sslctx);
    if (!ssl) return SSTK_SSL_ERROR_VERIFY;
    ret = wolfSSL_set_fd(ssl, cli->sock);
    if (ret != WOLFSSL_SUCCESS) goto err;

    ret = wolfSSL_connect(ssl);
    if (ret != WOLFSSL_SUCCESS) goto err;

    cli->ssl = ssl;

    cli->handler && cli->handler(cli, SSTK_TCP_EVT_SSL_OPEN, NULL, 0);

    return SSTK_TCP_ERR_NONE;

err:
    // 关闭 SSL 连接
    wolfSSL_shutdown(ssl);
    // 释放 SSL
    wolfSSL_free(ssl);

    cli->ssl = NULL;

    return SSTK_SSL_ERROR_VERIFY;
}

int sstk_ssl_on_cli_close(sstk_cli_t* cli) {
    if (!ssl_initialized) return SSTK_SSL_ERROR_UNINITIALIZED;

    if (cli->ssl) {
        SSL* ssl = (SSL*) cli->ssl;
        // 关闭 SSL 连接
        wolfSSL_shutdown(ssl);
        // 释放 SSL
        wolfSSL_free(ssl);
        cli->ssl = NULL;

        cli->handler && cli->handler(cli, SSTK_TCP_EVT_SSL_CLOSE, NULL, 0);
    }
    return SSTK_TCP_ERR_NONE;
}
