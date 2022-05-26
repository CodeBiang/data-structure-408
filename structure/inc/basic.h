#ifndef __BASIC_H__
#define __BASIC_H__

#include <stdbool.h>

typedef union {
    int i;
    double d;
    float f;
    bool b;
    const char* str;
    void* obj;
} basic_obj_t;

typedef bool (*basic_obj_equals_fn)(const basic_obj_t a, const basic_obj_t b);
typedef void (*basic_obj_accept_fn)(basic_obj_t obj);

#endif
