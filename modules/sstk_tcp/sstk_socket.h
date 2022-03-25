#ifndef __SSTK_SOCKET_H__
#define __SSTK_SOCKET_H__

#include <sstk_core.h>

#if SSTK_ARCH == SSTK_ARCH_UNIX
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct sockaddr remote_addr_t;
typedef struct sockaddr_in remote_addr_in_t;
typedef struct sockaddr_in6 remote_addr_in6_t;
typedef int socket_t;
#elif SSTK_ARCH == SSTK_ARCH_WIN32
#include <winsock2.h>  // winsock2.h 必须在 windows.h 之前

typedef struct sockaddr remote_addr_t;
typedef struct sockaddr_in remote_addr_in_t;
typedef struct sockaddr_in6 remote_addr_in6_t;
typedef SOCKET socket_t;
#elif SSTK_ARCH == SSTK_ARCH_TI_OS
#include <src/rtos/utils_common/include/params.h>  //包含了所有的参数声明和Dbg_printf的头文件
#include <ti/ndk/inc/netmain.h>
#define printf Dbg_printf

typedef SOCKET socket_t;
typedef struct sockaddr remote_addr_t;
typedef struct sockaddr_in remote_addr_in_t;
typedef struct sockaddr_in6 remote_addr_in6_t;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

char* inet_ntoa(struct in_addr in);

#define close(fd) fdClose(fd)
#define select fdSelect

#endif

#endif
