#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/wolfcrypt/random.h>
#include <wolfssl/wolfcrypt/types.h>

#define CERT_BUFFERED 1

#define CA_CERT_PATH "../certs/ca.crt"
#define SVR_CERT_PATH "../certs/server.crt"
#define SVR_KEY_PATH "../certs/server.key"

#define CA_CERT_BUF                                                    \
    "-----BEGIN CERTIFICATE-----"                                      \
    "MIIDkTCCAnmgAwIBAgIBAzANBgkqhkiG9w0BAQsFADB+MQswCQYDVQQGEwJDTjEO" \
    "MAwGA1UECAwFSGViZWkxFTATBgNVBAcMDFNoaWppYXpodWFuZzENMAsGA1UECgwE" \
    "V0hTVDENMAsGA1UECwwERURHRTELMAkGA1UEAwwCQ0ExHTAbBgkqhkiG9w0BCQEW" \
    "DjIwMDU1QHdoc3QuY29tMB4XDTIyMDMxODAxNDgzNloXDTMyMDMxNTAxNDgzNlow" \
    "fjELMAkGA1UEBhMCQ04xDjAMBgNVBAgMBUhlYmVpMRUwEwYDVQQHDAxTaGlqaWF6" \
    "aHVhbmcxDTALBgNVBAoMBFdIU1QxDTALBgNVBAsMBEVER0UxCzAJBgNVBAMMAkNB" \
    "MR0wGwYJKoZIhvcNAQkBFg4yMDA1NUB3aHN0LmNvbTCCASIwDQYJKoZIhvcNAQEB" \
    "BQADggEPADCCAQoCggEBANBD9JpGROROl6xbxfOBCqDRFnrJwC9v5e2OUl4+6yoT" \
    "OGoEUAlBVAgEb0QNx4GX56/1TUOexe0MOhGP5v6tJWoP4PBZcYyUxtfAH5HOZ31e" \
    "nPmVHTrvwAx6lQ6axuS6PFlC4V6Vl4uLx/calG0fda4xee0QyCJ2NwboojzG5zzx" \
    "3PJWnBmWFk1EjvgUt/oZc0I5E/1X6eK+w3Kiax3LGtvJT/dmSeeqbtrrn2Uw8gSF" \
    "UYvgNJDXGRFvpavm+KZkbxNsYtp26LxvoYnA938GNpb9Xf6p4Zx8BhKjJaBkgb/t" \
    "mi2aKENa6gxYHzg4CaZqklnMvfMZE+WH1nwHCOTpCMMCAwEAAaMaMBgwCQYDVR0T" \
    "BAIwADALBgNVHQ8EBAMCBeAwDQYJKoZIhvcNAQELBQADggEBAD/BKyzOEwXdQHDL" \
    "kBxgFo2poztTc874mEhRiCGNOB3ArCMcdv/UA+dDsbsDnmGq9h9wbuNk5DARYifL" \
    "Qw1rxWB6zIfvNah9kYfePRHmigqXWXQ2r9MyFfW3etKN91F3Gitka3UUaU7dlakN" \
    "+YkIlB3bXw22o+6UAOdBsTaUEJnlEpQaIk969SHCWskyCBcZlVVqO0hski+RAMuH" \
    "aiK8Uawn2x7Z3v23NavcUyhkKOjfEqDkk/0/KEHcwGDbcqnQsHJsyiVsQofQkIZd" \
    "86TluaT9k5p0aL7/VtQNwR+G+8eS3bq1nGQJD6QOOlQkqf/UoFxefo+dzxrt74pt" \
    "r7W1N2E="                                                         \
    "-----END CERTIFICATE-----"

#define SVR_CERT_BUF                                                   \
    "-----BEGIN CERTIFICATE-----"                                      \
    "MIIDljCCAn6gAwIBAgIBAzANBgkqhkiG9w0BAQsFADB+MQswCQYDVQQGEwJDTjEO" \
    "MAwGA1UECAwFSGViZWkxFTATBgNVBAcMDFNoaWppYXpodWFuZzENMAsGA1UECgwE" \
    "V0hTVDENMAsGA1UECwwERURHRTELMAkGA1UEAwwCQ0ExHTAbBgkqhkiG9w0BCQEW" \
    "DjIwMDU1QHdoc3QuY29tMB4XDTIyMDMxODAxNDgzNloXDTIyMDQxNzAxNDgzNlow" \
    "gYIxCzAJBgNVBAYTAkNOMQ4wDAYDVQQIDAVIZWJlaTEVMBMGA1UEBwwMU2hpamlh" \
    "emh1YW5nMQ0wCwYDVQQKDARXSFNUMQ0wCwYDVQQLDARFREdFMQ8wDQYDVQQDDAZT" \
    "RVJWRVIxHTAbBgkqhkiG9w0BCQEWDjIwMDU1QHdoc3QuY29tMIIBIjANBgkqhkiG" \
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvod132hw72XaIeHapA9xsHzFq9c/D0GnaypQ" \
    "VVL4aQOiQlNoqrpJOHBdZTiQHVNtq+WHqz2cMoefKA4aJb3lZRGC+adoekUpFeRJ" \
    "m2wq7TXsFcub88U5jYWQz6lM35pB8UV9615QNw+gC7DboT902oc1kVI3z6ncYQXo" \
    "GVIH5UaASJON08t22XKPq5BDjtRc2VkYTFJL32jaSqwOgb85q7B2S+tTZumaw1Xw" \
    "WeoCVMK3g0oRTbOcVgcYjTc0Ig/vsw/+On6vLicdEvSNOTPLerwCtDIuZRN+bGge" \
    "rGC3YhRCnb7x1MWNZXUxCQHFf6+MDTZHxsHdbroPoEmRWo/XRwIDAQABoxowGDAJ" \
    "BgNVHRMEAjAAMAsGA1UdDwQEAwIF4DANBgkqhkiG9w0BAQsFAAOCAQEAgx/PMexs" \
    "a3dGYLREya3v1P75wYlDpH/G31h2a9WHBMt37u9kdYQCLEsDp7WNKZbYX8Y5FJxa" \
    "GBAoM7K+ltJYXCKKe/tULn4x0ol8EU0VxVZF9BIZ3H0FEP7Wqt9jIrwQ1HlWzlZQ" \
    "Ywp2JNcowudXmE4BdcOiPF/Nosfap0/32o9KMhATtmC12zU5amyRIfkwn1PJWKnP" \
    "oPDejM8cKSDdbwJdJVP03fSMhd06J7qjcX2P39zekymBBEVI2OKxobzmMgP9o+Z7" \
    "U1obSRHcXAkcyisOHnuv9kk9arWLwth3RrV5D9l3HZ89Q9debuJnXqVlEk3YdcL9" \
    "5VUnI27FCq9jiw=="                                                 \
    "-----END CERTIFICATE-----"

#define SVR_KEY_BUF                                                    \
    "-----BEGIN ENCRYPTED PRIVATE KEY-----"                            \
    "MIIFHDBOBgkqhkiG9w0BBQ0wQTApBgkqhkiG9w0BBQwwHAQIWrKAEBFUsiICAggA" \
    "MAwGCCqGSIb3DQIJBQAwFAYIKoZIhvcNAwcECDfqXwBzOfThBIIEyO1smLg04hmD" \
    "VhonKM7c2tnuIW6miLmT/o3zYxfYRWdJ1WbYuGSIaTK/zSCtyhvK5ygzEKFdSPiL" \
    "LeGmyd4VIz8v1YxtXYU8zt58H/zDn3R0SAxv4uJMOu2UVkUZYfZSIVpZ5ZLcj+X8" \
    "sTHR1sHtheBSeDAyJnHEzN+Vq7H24RjMC8F0Wm+/WJXQKL4ajnEusXFIpPC+7Apk" \
    "ngrufpWtST3OBA28pT6y/xF04R4BJOwC3YOd849zUbo02VHCFLq0wCr5wh85O6xf" \
    "8raguadBrbRaJAwklVTHHnLLnYFT3/bZ7o4Tcz8SfrCLVzw5He4qNHX9HnS85K7m" \
    "5B5y2HwPQJRIv2XgN9ScUuQ8eVCPu4BZEH/VFbTh+Jj8r+uKdLeXZ6fuBnPTT8YR" \
    "L4le70keq10kFb1mKCdqMtStEMwa34zxWr5PRSdhU/hs6OCG/Z36a3Xy7Ti6ZAv2" \
    "dGDpdqOkevrOGM2U+jFFG5JpuQTIKNCRyZdZ2jl1TUtK2O2qjm1J5eaXHgPJl6D7" \
    "9ekBsvKCXoOWjYUNHy047C1SX8oJNabnojK1bserhdTPEgCUkw75+8QoX2/YEtKz" \
    "HKs9gjf6dd2J4Dq3tCrkd1/+J5jpk9++SHu6vUnnzYvsYn3ly4+qcFYrl5OAmrAc" \
    "3c20nA+AVrQQabxks782qOFH6KunKv55rrKGm7wr/KxizHVZ4xY+h4FhHTfawE32" \
    "mIgv9UpIBxJB4uX5Un0ywGHQxBp8Vx41XdSS9QXiqNJuan9SNM6LpzKVEXSsXTxX" \
    "cUTMvNKrNzGJkzPwhE7M1X0EMSZkM2qK0IvrwVHTe3Tb0Tvjd6PO5clRVz3HX96f" \
    "UtZtojQPkHUJQcb3KUl3WwQAXTWoycIhlQVXwq9zyxqdTxOFzCzSyLZLDWgSUYQ0" \
    "jBkViM0GUhUH/++2kXZtHrwcfSuo3QjktldvElZcf6jMEYrtVhuQpYQX24k3ZCYC" \
    "auJW4/F6KHflNO2XJ5OzV2ICkzpClLon8WmA4mUnw0GBY5SGOWk/gOLHGtAjvKUe" \
    "0WmRQ2E8aIYZEPruWzvKQbwc41rTyiTISG6ncONsGVTh2FG2BOHDJ8hks8FTAO18" \
    "fbDUyUvCnaVKicJDqFSK38rvg1C2rV5iUKJV0qXaaz9AWuzbFKPwutufBmsGunZX" \
    "N0zwxUSCmu1DDZkt13dg3buOOp74LMD9vUxFay0WGdUXF65DTsrFi/tPBDyxgh9L" \
    "8z6kACuVKPI2LP4jeImqk8dky0l69EY7bgUMf9Aga6f8uBV1+Uqmm30d7/gtdmm9" \
    "e5jrjY4XJj6396zLDVuKA6qGr0hF2MVHc2O+rKKNqh7Rts1guCUK/r4mXBQOyho/" \
    "xwVvhkDM0XdTD6cMQpKp965gBq3yN/vQH5xtw0RCfWig/R9Zat3t8yp13J4+Of9A" \
    "RDem9G5msyyZhySSrEpMpM1Vyg+PUAb1oO0Qn0GO02mtsFcNAGtq2FmWLqTBGl9E" \
    "QthddKgfGjPgnQBxVDWuuNDz9FINsDRxvntk3FlkwAfDu4YaIg9mTN5zAvKx7r8V" \
    "VD0tbxkws3H7SMItqNYgswBDCfa3PkbPnDuYc0/xSX3qjsYce45v2KvW6SX/l5rE" \
    "sRJacgXx84aQtWbnfCW7fA=="                                         \
    "-----END ENCRYPTED PRIVATE KEY-----"

#define CA_CERT_BUF_SIZE (sizeof(CA_CERT_BUF) - 1)
#define SVR_CERT_BUF_SIZE (sizeof(SVR_CERT_BUF) - 1)
#define SVR_KEY_BUF_SIZE (sizeof(SVR_KEY_BUF) - 1)

#define MAXBUF 1024

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
    snprintf(buf, len, "%s", (char*) password);
    printf("pass callback\n");
    return strlen(buf);
}

int main(int argc, char* argv[]) {
    int ret;

    wolfSSL_Init();

    WOLFSSL_CTX* ctx = NULL;
    WOLFSSL* ssl = NULL;

    ctx = wolfSSL_CTX_new(wolfTLSv1_3_server_method());
    wolfSSL_CTX_set_cipher_list(ctx, "TLS13-AES128-GCM-SHA256");
    // 证书口令
    wolfSSL_CTX_set_default_passwd_cb(ctx, pass_cb);
    // 等价于
    wolfSSL_CTX_set_default_passwd_cb_userdata(ctx, "123456");

    // 配置双向认证
    // SSL_VERIFY_PEER---要求对证书进行认证, 没有证书也会放行
    // SSL_VERIFY_FAIL_IF_NO_PEER_CERT---要求客户端需要提供证书, 但验证发现单独使用没有证书也会放行
    // wolfSSL_CTX_set_verify(ctx, WOLFSSL_VERIFY_DEFAULT, NULL);
    wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
#if CERT_BUFFERED
    if (wolfSSL_CTX_load_verify_buffer(ctx, CA_CERT_BUF, CA_CERT_BUF_SIZE, SSL_FILETYPE_PEM) != SSL_SUCCESS) {
#else
    if (wolfSSL_CTX_load_verify_locations(ctx, CA_CERT_PATH, NULL) != SSL_SUCCESS) {
#endif
        perror("根证书异常");
        exit(-1);
    }

    // 上下文加载 证书
#if CERT_BUFFERED
    if (wolfSSL_CTX_use_certificate_buffer(ctx, SVR_CERT_BUF, SVR_CERT_BUF_SIZE, SSL_FILETYPE_PEM) != SSL_SUCCESS) {
#else
    if (wolfSSL_CTX_use_certificate_file(ctx, SVR_CERT_PATH, SSL_FILETYPE_PEM) != SSL_SUCCESS) {
#endif
        perror("证书异常");
        exit(-1);
    }

    wolfSSL_Debugging_ON();
    // 上下文加载 key
#if CERT_BUFFERED
    if ((ret = wolfSSL_CTX_use_PrivateKey_buffer(ctx, SVR_KEY_BUF, SVR_KEY_BUF_SIZE, SSL_FILETYPE_PEM)) != SSL_SUCCESS) {
#else
    if ((ret = wolfSSL_CTX_use_PrivateKey_file(ctx, SVR_KEY_PATH, SSL_FILETYPE_PEM)) != SSL_SUCCESS) {
#endif
        printf("private key error %d\n", ret);
        exit(-2);
    }
    wolfSSL_Debugging_OFF();

    // 检查用户私钥是否正确
    if (wolfSSL_CTX_check_private_key(ctx) != WOLFSSL_SUCCESS) {
        printf("Error check\n");
        exit(-3);
    }

    int sockfd;
    // 开启一个 socket 监听
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    } else
        printf("socket created\n");

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void*) &opt, sizeof(int));

    socklen_t len;
    char buf[MAXBUF + 1];
    struct sockaddr_in serv_addr, cli_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_port = htons(10101);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    } else
        printf("binded\n");

    if (listen(sockfd, 10) == -1) {
        perror("listen");
        exit(1);
    } else
        printf("begin listen\n");

    while (1) {
        int clifd;
        len = sizeof(struct sockaddr);
        // 等待客户端连上来
        if ((clifd = accept(sockfd, (struct sockaddr*) &cli_addr, &len)) == -1) {
            perror("accept");
            exit(errno);
        } else
            printf("server: got connection from %s, port %d, socket %d\n",
                   inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), clifd);

        printf("accept\n");
        // 基于 ctx 产生一个新的 SSL
        ssl = wolfSSL_new(ctx);
        // 将连接用户的 socket 加入到 SSL
        wolfSSL_set_fd(ssl, clifd);
        printf("accept \n");
        int ssl_ret = wolfSSL_accept(ssl);
        printf("accept ret %d\n", ssl_ret);
        // 建立 SSL 连接
        if (ssl_ret != WOLFSSL_SUCCESS) {
            char ebuf[256];
            unsigned long eret = wolfSSL_ERR_get_error();

            printf("current error %d %ld, %s\n", ssl_ret, eret,
                   wolfSSL_ERR_error_string(eret, ebuf));
            close(clifd);
            break;
        }
        show_certs(ssl);

        // 开始处理每个新连接上的数据收发
        bzero(buf, MAXBUF + 1);
        strcpy(buf, "server->client");
        // 发消息给客户端
        len = wolfSSL_write(ssl, buf, strlen(buf));

        if (len <= 0) {
            printf("消息'%s'发送失败!错误代码是%d, 错误信息是'%s'\n", buf, errno, strerror(errno));
            goto finish;
        } else
            printf("消息'%s'发送成功, 共发送了%d个字节!\n", buf, len);

        bzero(buf, MAXBUF + 1);
        // 接收客户端的消息
        len = wolfSSL_read(ssl, buf, MAXBUF);
        if (len > 0) {
            printf("接收消息成功:'%s', 共%d个字节的数据\n", buf, len);
        } else
            printf("消息接收失败!错误代码是%d, 错误信息是'%s'\n", errno, strerror(errno));

    // 处理每个新连接上的数据收发结束
    finish:
        // 关闭 SSL 连接
        wolfSSL_shutdown(ssl);
        // 释放 SSL
        wolfSSL_free(ssl);
        // 关闭 socket
        close(clifd);
        break;
    }
    // 测试一个基础服务端
    wolfSSL_CTX_free(ctx);
    wolfSSL_Cleanup();
    return 0;
}
