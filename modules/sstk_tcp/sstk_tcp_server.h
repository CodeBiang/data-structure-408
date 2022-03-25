#ifndef __SSTK_TCP_SERVER_H__
#define __SSTK_TCP_SERVER_H__

#include "sstk_tcp_common.h"

typedef struct sstk_conn_s sstk_conn_t;
typedef struct sstk_server_s sstk_server_t;
typedef struct sstk_server_opt_s sstk_server_opt_t;

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
typedef int (*sstk_server_evt_handler)(sstk_conn_t* c, int e, void* p, size_t l,
                                       const sstk_server_t* s);

/**
 * 连接过滤器, 原型函数
 * 用于遍历、批量处理连接
 *
 * @param c 连接实体
 * @param s 服务实体
 * @param user_data 用户数据
 *
 * @return true 保留, false 过滤
 */
typedef sstk_bool_t (*sstk_conn_filter_fn)(const sstk_conn_t* c, const sstk_server_t* s,
                                           void* user_data);

/* tcp 连接 */
struct sstk_conn_s {
    /* public */
    void* user_data;  // 用户信息
    // use heap mem
    remote_addr_t* remote_addr;  // 远程地址

    volatile unsigned in_use : 1;   // 可用标志
    volatile unsigned is_ipv6 : 1;  // 是否为ipv6连接

    /* private */
    socket_t sock;      // 套接字
    sstk_conn_t* prev;  // prev 节点
    sstk_conn_t* next;  // next 节点
    void* ssl;          // ssl

    sstk_server_t* serv;  // 所属服务
};

/* 服务配置 */
struct sstk_server_opt_s {
    unsigned short port;  // 端口

    sstk_ssl_opt_t ssl_opt;  // ssl 配置

    unsigned ipv6_support : 1;  // 启用 ipv4/ipv6 双监听
    unsigned ssl_support : 1;   // 启用 ssl 加密传输
};

struct sstk_server_s {
    /* public */
    sstk_server_opt_t opt;  // 配置
    void* user_data;        // 用户数据
    char* bufptr;           // buffer 指针, 空指针会默认从堆中开辟
    size_t bufsize;         // bufsize 缓冲区大小

    unsigned buf_need_free : 1;    // buf 需要释放
    volatile unsigned in_use : 1;  // 状态, true 已开启, false 已关闭

    sstk_server_evt_handler handler;  // 事件驱动模型下的 tcp server 事件回调处理器

    /* private */
    socket_t sock;   // ipv4套接字
    socket_t sock6;  // ipv6套接字

    void* ctx;     // 服务内部上下文
    void* sslctx;  // ssl

    sstk_conn_t* conns;  // 连接头节点
};

#define SSTK_PEM_INTIALIZER \
    { NULL, 0 }
#define SSTK_SSL_OPT_INITIALIZER                                                         \
    {                                                                                    \
        SSTK_PEM_INTIALIZER, SSTK_PEM_INTIALIZER, SSTK_PEM_INTIALIZER, NULL, 0, 0, NULL, \
            SSTK_TLS_V11, 0                                                              \
    }
#define SSTK_SERVER_OPT_INITIALIZER \
    { 0, SSTK_SSL_OPT_INITIALIZER, 0, 0 }
#define SSTK_SERVER_INITIALIZER \
    { SSTK_SERVER_OPT_INITIALIZER, NULL, NULL, 0, 0, 0, NULL, 0, 0, NULL, NULL, NULL }

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

#endif
