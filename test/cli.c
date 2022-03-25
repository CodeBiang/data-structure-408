#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WOLFSSL_DEBUG
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/wolfcrypt/random.h>
#include <wolfssl/wolfcrypt/types.h>

#define MAXBUF 1024
// #define CA_CERT_PATH "../wolfssl-5.2.0/certs/ca-cert.pem"
// #define CLI_CERT_PATH "../wolfssl-5.2.0/certs/client-cert.pem"
// #define CLI_KEY_PATH "../wolfssl-5.2.0/certs/client-key.pem"
#define CA_CERT_PATH "../certs/ca.crt"
#define CLI_CERT_PATH "../certs/client.crt"
#define CLI_KEY_PATH "../certs/client.key"
// #define CLI_KEY_PATH "../certs/client_rsa_private.pem.unsecure"

static void show_certs(WOLFSSL* ssl) {
    WOLFSSL_X509* cert;
    char* line;

    cert = wolfSSL_get_peer_certificate(ssl);
    // SSL_get_verify_result()是重点, SSL_CTX_set_verify()只是配置启不启用并没有执行认证,
    // 调用该函数才会真证进行证书认证 如果验证不通过, 那么程序抛出异常中止连接
    if (wolfSSL_get_verify_result(ssl) == WOLFSSL_SUCCESS) {
        printf("证书验证通过\n");
    }
    if (cert != NULL) {
        printf("数字证书信息:\n");
        line = wolfSSL_X509_NAME_oneline(wolfSSL_X509_get_subject_name(cert), 0, 0);
        printf("证书: %s\n", line);
        free(line);
        line = wolfSSL_X509_NAME_oneline(wolfSSL_X509_get_issuer_name(cert), 0, 0);
        printf("颁发者: %s\n", line);
        free(line);
        wolfSSL_X509_free(cert);
    } else
        printf("无证书信息!\n");
}

static int pass_cb(char* buf, int len, int verify, void* password) {
    snprintf(buf, len, "666666");
    return strlen(buf);
}

int main(int argc, char* argv[]) {
    wolfSSL_Init();

    WOLFSSL_CTX* ctx = NULL;
    WOLFSSL* ssl = NULL;

    ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
    wolfSSL_CTX_set_cipher_list(ctx, "TLS13-AES128-GCM-SHA256");
    // 证书口令
    wolfSSL_CTX_set_default_passwd_cb(ctx, pass_cb);

    // 配置双向认证
    // SSL_VERIFY_PEER---要求对证书进行认证, 没有证书也会放行
    // SSL_VERIFY_FAIL_IF_NO_PEER_CERT---要求客户端需要提供证书, 但验证发现单独使用没有证书也会放行
    wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
    if (wolfSSL_CTX_load_verify_locations(ctx, CA_CERT_PATH, NULL) != SSL_SUCCESS) {
        perror("根证书异常");
        exit(-1);
    }

    // 上下文加载 证书
    if (wolfSSL_CTX_use_certificate_file(ctx, CLI_CERT_PATH, SSL_FILETYPE_PEM)
        != SSL_SUCCESS) {
        perror("证书异常");
        exit(-1);
    }

    // 上下文加载 key
    if (wolfSSL_CTX_use_PrivateKey_file(ctx, CLI_KEY_PATH, SSL_FILETYPE_PEM)
        != SSL_SUCCESS) {
        perror("key异常");
        exit(-2);
    }

    // 检查用户私钥是否正确
    if (!wolfSSL_CTX_check_private_key(ctx)) {
        perror("检测异常");
        exit(-3);
    }
    

    int sockfd;
    // 开启一个 socket 监听
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        goto exit1;
    } else
        printf("socket created\n");

    char buf[MAXBUF + 1];
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(10101);
    serv_addr.sin_addr.s_addr = inet_addr("172.16.12.128");

    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(-1);
    }

    ssl = wolfSSL_new(ctx);

    wolfSSL_set_fd(ssl, sockfd);

    int sslret = wolfSSL_connect(ssl);
    if (sslret != WOLFSSL_SUCCESS) {
        printf("connect tlsv13 failed %d\n", sslret);
        goto exit0;
    }
    // 测试接收
    int n = wolfSSL_read(ssl, buf, MAXBUF);
    buf[n] = '\0';
    printf("%s 接受\n", buf);
    wolfSSL_write(ssl, buf, n);

exit0:
    wolfSSL_shutdown(ssl);
    wolfSSL_free(ssl);
    close(sockfd);
exit1:
    // 测试一个基础服务端
    wolfSSL_CTX_free(ctx);
    wolfSSL_Cleanup();
    return 0;
}