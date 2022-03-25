#ifndef __SSTK_TCP_CLIENT_H__
#define __SSTK_TCP_CLIENT_H__

#include "sstk_tcp_common.h"

typedef struct sstk_cli_s sstk_cli_t;
typedef struct sstk_cli_opt_s sstk_cli_opt_t;

/**
 * 服务事件驱动模型，回调函数
 *
 * @param c 连接实体
 * @param e 事件码
 * @param p 数据源地址
 * @param l 数据长度
 * @param s 服务实体
 *
 * @return errno (目前无意义)
 */
typedef int (*sstk_cli_evt_handler)(sstk_cli_t* c, int e, void* p, size_t l);

struct sstk_cli_opt_s {
    remote_addr_t* serv_addr;  // 服务器地址
    sstk_ssl_opt_t ssl_opt;    // ssl 配置
    int reconnect_interval;    // 重连时间间隔

    unsigned ipv6_enable : 1;     // 启用 ipv6
    unsigned ssl_enable : 1;      // 启用 ssl 加密传输
    unsigned auto_reconnect : 1;  // 启用自动重连
};

struct sstk_cli_s {
    /* public */
    sstk_cli_opt_t opt;
    char* bufptr;    // buffer 指针, 空指针会默认从堆中开辟
    size_t bufsize;  // bufsize 缓冲区大小

    /* public callbacks */
    sstk_cli_evt_handler handler;

    unsigned buf_need_free : 1;       // allocated buffer, 缓冲区需要释放
    volatile unsigned in_use : 1;     // 可用标志
    volatile unsigned connected : 1;  // 已连接标志

    /* private */
    socket_t sock;  // 套接字
    void* sslctx;   // sslctx
    void* ssl;      // ssl
    void* ctx;      // ctx
};

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

#endif
