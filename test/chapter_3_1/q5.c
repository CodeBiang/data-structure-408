#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 5

struct {
    int mem[MAX_SIZE];
    int top[2];
} s;

static bool s1_push(int elem) {
    if (s.top[0] + 1 >= s.top[1]) return false;

    s.mem[++s.top[0]] = elem;
    return true;
}

static bool s2_push(int elem) {
    if (s.top[0] + 1 >= s.top[1]) return false;

    s.mem[--s.top[1]] = elem;
    return true;
}

static bool s1_pop(int* elem) {
    if (s.top[0] != -1) {
        *elem = s.mem[s.top[0]--];
        return true;
    }
    return false;
}

static bool s2_pop(int* elem) {
    if (s.top[1] != MAX_SIZE) {
        *elem = s.mem[s.top[1]++];
        return true;
    }
    return false;
}

static void memprint() {
    for (int i = 0; i < MAX_SIZE; i++) {
        printf("%d ", s.mem[i]);
    }
    printf("\n");
}

int main() {
    int temp;
    s1_push(1);
    s1_push(2);
    s1_push(3);
    s2_push(4);
    s2_push(5);
    memprint();
    s2_push(5);
    memprint();
    s2_pop(&temp);
    s2_push(6);
    memprint();
    s1_pop(&temp);
    s2_push(77);
    memprint();
    return 0;
}
