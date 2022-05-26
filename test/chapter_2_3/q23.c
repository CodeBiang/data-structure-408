#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_s {
    char data;
    struct node_s* next;
};

static struct node_s* suffix_node(struct node_s* str1, struct node_s* str2) {
    int m, n;
    struct node_s* p;
    struct node_s* q;

    // 测长度
    for (p = str1, m = 0; p; p = p->next, m++)
        ;

    for (q = str2, n = 0; q; q = q->next, n++)
        ;

    // 对齐
    for (p = str1; n < m; n++) p = p->next;

    for (q = str2; m < n; m++) q = q->next;

    // 对齐后比对指针
    while (p && q && p != q) {
        p = p->next;
        q = q->next;
    }

    return (p && p == q) ? p : NULL;
}

static inline struct node_s* node_dump(char value) {
    struct node_s* ret = (struct node_s*) malloc(sizeof(struct node_s));
    ret->data = value;
    ret->next = NULL;
    return ret;
}

int main() {
    struct node_s* str1 = node_dump(0);
    struct node_s* str2 = node_dump(0);

    struct node_s* suffix = node_dump('i');
    suffix->next = node_dump('n');
    suffix->next->next = node_dump('g');

    str1->next = node_dump('l');
    str1->next->next = node_dump('o');
    str1->next->next->next = node_dump('a');
    str1->next->next->next->next = node_dump('d');
    str1->next->next->next->next->next = suffix;
    str2->next = node_dump('b');
    str2->next->next = node_dump('e');
    str2->next->next->next = suffix;

    printf("%c\n", suffix_node(str1, str2)->data);
    return 0;
}
