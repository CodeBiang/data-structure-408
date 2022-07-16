#include <string.h>

#include "linked_list.h"
#include "sq_queue.h"
#include "stdio.h"
// static void obj_print(basic_obj_t obj) { printf("%d ", obj.i); }

int* get_nextval(const char* pattern, int plen) {
    int* nextval = (int*) malloc(sizeof(int) * plen);

    int i = 0, j = -1;

    nextval[0] = -1;

    while (i < plen - 1) {
        if (j == -1 || pattern[i] == pattern[j]) {
            ++i;
            ++j;

            // 优化next值
            // 相等的值直接赋值next[j]可以避免多次回溯
            if (pattern[i] != pattern[j])
                nextval[i] = j;
            else
                nextval[i] = nextval[j];
        } else
            j = nextval[j];
    }

    for (int i = 0; i < plen; i++) {
        printf("%d ", nextval[i]);
    }
    printf("\n");
    return nextval;
}

int* get_next(const char* pattern, int plen) {
    int* next = (int*) malloc(sizeof(int) * plen);

    int i = 0, j = -1;

    next[0] = -1;

    while (i < plen - 1) {
        if (j == -1 || pattern[i] == pattern[j]) {
            next[++i] = ++j;
        } else
            j = next[j];
    }

    for (int i = 0; i < plen; i++) {
        printf("%d ", next[i]);
    }
    printf("\n");
    return next;
}

int kmp(const char* str, int slen, const char* p, int plen) {
    int i = 0, j = -1;
    int* next = get_next(p, plen);

    while (i < slen && j < plen) {
        if (j == -1 || str[i] == p[j]) {
            j++;
            i++;
        } else {
            j = next[j];
        }
    }

    // 模式匹配结束, 返回匹配到的起始索引
    if (j == plen)
        return i - plen;
    else
        return -1;
}

int main() {
    const char* str = "123123123";
    const char* pattern = "ABABAAABABAA";
    const char* pattern1 = "aaaab";
    int len = strlen(str);

    // printf("%d\n", kmp(str, strlen(str), pattern, strlen(pattern)));

    get_next(pattern, strlen(pattern));
    get_nextval(pattern, strlen(pattern));
    get_next(pattern1, strlen(pattern1));
    get_nextval(pattern1, strlen(pattern1));
    return 0;
}
