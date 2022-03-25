#ifndef __SSTK_SSL_H__
#define __SSTK_SSL_H__

#include <stddef.h>

typedef struct sstk_ssl_opt_s sstk_ssl_opt_t;
typedef struct sstk_ssl_pem_s sstk_ssl_pem_t;

/* pem 格式字符串 */
struct sstk_ssl_pem_s {
    const char* pembuf;  // pem 文件buffer头指针
    size_t size;         // 长度
};

/* tls 版本 */
enum sstk_tls_version_e {
    SSTK_TLS_V11 = 0,  // TLSv1.1
    SSTK_TLS_V12,      // TLSv1.2
    SSTK_TLS_V13       // TLSv1.3
};

/* ssl/tls 配置 */
struct sstk_ssl_opt_s {
    sstk_ssl_pem_t ca;       // ca 证书, 配置 client_auth 后启用双向认证
    sstk_ssl_pem_t cert;     // 服务器证书
    sstk_ssl_pem_t key;      // 服务器私钥
    const char* key_secret;  // key 口令
    int key_secret_size;     // key 口令长度

    unsigned int session_timeout;  // ssl 会话超时时间, 秒, 默认500

    const char* cipher_list;  // 支持加密算法列表

    enum sstk_tls_version_e version : 4;  // tls版本
    unsigned client_auth : 1;             // 双向认证
};

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

#endif
