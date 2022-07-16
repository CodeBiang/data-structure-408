#ifndef __STR_H__
#define __STR_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct str_s str_t;

struct str_s {
    char* value;
    int length;
};

bool str_init(str_t* str);
bool str_destroy(str_t* str);
int str_index_of(str_t* str, const char* pattern, size_t plen);

#endif 
