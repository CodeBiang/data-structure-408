/* private header */
#ifndef __SSTK_TCP_SSL_IMPL_H__
#define __SSTK_TCP_SSL_IMPL_H__

#include <sstk_tcp_client.h>
#include <sstk_tcp_server.h>

int sstk_ssl_send(void* ssl, const void* data, size_t size, int flags);
int sstk_ssl_recv(void* ssl, void* data, size_t size, int flags);
int sstk_ssl_on_conn_open(sstk_server_t* serv, void* sctx, sstk_conn_t* conn);
int sstk_ssl_on_conn_close(sstk_server_t* serv, void* sctx, sstk_conn_t* conn);
int sstk_ssl_server_ctx_init(sstk_server_t* serv);
int sstk_ssl_server_ctx_cleanup(sstk_server_t* serv);

int sstk_ssl_cli_ctx_init(sstk_cli_t* cli);
int sstk_ssl_cli_ctx_cleanup(sstk_cli_t* cli);
int sstk_ssl_on_cli_open(sstk_cli_t* cli);
int sstk_ssl_on_cli_close(sstk_cli_t* cli);

#endif
