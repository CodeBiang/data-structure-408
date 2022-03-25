/**
 * @file    sstk_string.h
 * @date    2021/5/12
 * @author  赵陈淏
 * @brief   字符串结构
 * 
 * @todo    保证内存安全前提，实现并提供堆内字符串的各项操作
 */
#ifndef __SSTK_STRING_H_
#define __SSTK_STRING_H_

#include <sstk_core.h>

typedef struct {
    size_t len;
    unsigned char* data;
} sstk_str_t;

#define sstk_string(str) \
    { sizeof(str) - 1, (unsigned char*) str }
#define sstk_null_string \
    { 0, NULL }
#define sstk_str_set(str, text)    \
    (str)->len = sizeof(text) - 1; \
    (str)->data = (unsigned char*) text
#define sstk_str_null(str) \
    (str)->len = 0;        \
    (str)->data = NULL

#define sstk_tolower(c) \
    (unsigned char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define sstk_toupper(c) \
    (unsigned char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

void sstk_strlow(unsigned char* dst, unsigned char* src, size_t n);

#define sstk_strncmp(s1, s2, n) strncmp((const char*) s1, (const char*) s2, n)

#define sstk_strcmp(s1, s2) strcmp((const char*) s1, (const char*) s2)

#define sstk_strstr(s1, s2) strstr((const char*) s1, (const char*) s2)
#define sstk_strlen(s) strlen((const char*) s)

size_t sstk_strnlen(unsigned char* p, size_t n);

#define sstk_memzero(buf, n) (void) memset(buf, 0, n)
#define sstk_memset(buf, c, n) (void) memset(buf, c, n)

#define sstk_memcpy(dst, src, n) (void) memcpy(dst, src, n)
#define sstk_cpymem(dst, src, n) (((unsigned char*) memcpy(dst, src, n)) + (n))

#define sstk_copy sstk_cpymem

#define sstk_memmove(dst, src, n) (void) memmove(dst, src, n)
#define sstk_movemem(dst, src, n) (((unsigned char*) memmove(dst, src, n)) + (n))

#define sstk_memcmp(s1, s2, n) memcmp((const char*) s1, (const char*) s2, n)

unsigned char* sstk_cpystrn(unsigned char* dst, unsigned char* src, size_t n);

sstk_int_t sstk_strcasecmp(unsigned char* s1, unsigned char* s2);
sstk_int_t sstk_strncasecmp(unsigned char* s1, unsigned char* s2, size_t n);

unsigned char* sstk_strnstr(unsigned char* s1, char* s2, size_t n);

unsigned char* sstk_strstrn(unsigned char* s1, char* s2, size_t n);
unsigned char* sstk_strcasestrn(unsigned char* s1, char* s2, size_t n);
unsigned char* sstk_strlcasestrn(unsigned char* s1, unsigned char* last,
                                 unsigned char* s2, size_t n);

void sstk_sort(void* base, size_t n, size_t size,
               sstk_int_t (*cmp)(const void*, const void*));
#define sstk_qsort qsort

#endif
