#ifndef __BASIC_H__
#define __BASIC_H__

#include <stdbool.h>

typedef union {
    int i;
    char c;
    double d;
    float f;
    bool b;
    const char* str;
    void* obj;
} basic_obj_t;

typedef bool (*basic_obj_equals_fn)(const basic_obj_t* a, const basic_obj_t* b);
typedef void (*basic_obj_accept_fn)(basic_obj_t* obj);

#define basic_param_wrap(field, val) ((basic_obj_t[]){{.field = val}})
#define basic_int_wrap(val) basic_param_wrap(i, val)
#define basic_char_wrap(val) basic_param_wrap(c, val)
#define basic_float_wrap(val) basic_param_wrap(f, val)
#define basic_double_wrap(val) basic_param_wrap(d, val)
#define basic_str_wrap(val) basic_param_wrap(str, val)
#define basic_ptr_wrap(val) basic_param_wrap(obj, val)

#endif
