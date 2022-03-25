#ifndef __SSTK_MD5_H_
#define __SSTK_MD5_H_

#include <sstk_core.h>

typedef struct {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    uint8_t    buffer[64];
} sstk_md5_t;


void sstk_md5_init(sstk_md5_t *ctx);
void sstk_md5_update(sstk_md5_t *ctx, const void *data, size_t size);
void sstk_md5_final(uint8_t result[16], sstk_md5_t *ctx);

#endif
