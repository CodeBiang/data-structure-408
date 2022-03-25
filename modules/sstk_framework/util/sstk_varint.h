/**
 * @file    
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   varint 工具
 */
#ifndef __SSTK_VARINT_H_
#define __SSTK_VARINT_H_

#include <stddef.h>

static inline int sstk_varint_encode(unsigned int value, unsigned char data[5]) {
    int count = 0;
    do {
        data[count] = (unsigned char) ((value & 0x7F) | 0x80);
        count++;
    } while ((value >>= 7) != 0);
    data[count - 1] &= 0x7F;

    return count;
}

static inline int sstk_varint_decode(unsigned char* data, unsigned int* val, size_t size) {
    unsigned int value = data[0];
    int offset = 1;

    if (size == 0) return -1;
    
    if ((value & 0x80) == 0) {
        *val = value;
        return offset;
    }
    offset++;

    if (size < offset) return -1;

    value &= 0x7F;
    unsigned int chunk = data[1];
    value |= (chunk & 0x7F) << 7;
    if ((chunk & 0x80) == 0) {
        *val = value;
        return offset;
    }
    offset++;

    if (size < offset) return -1;

    chunk = data[2];
    value |= (chunk & 0x7F) << 14;
    if ((chunk & 0x80) == 0) {
        *val = value;
        return offset;
    }
    offset++;

    if (size < offset) return -1;

    chunk = data[3];
    value |= (chunk & 0x7F) << 21;
    if ((chunk & 0x80) == 0) {
        *val = value;
        return offset;
    }
    offset++;

    if (size < offset) return -1;
    
    chunk = data[4];
    value |= chunk << 28;
    if ((chunk & 0xF0) == 0) {
        *val = value;
        return offset;
    }
    return -1;
}

#endif  // __SSTK_VARINT_H_