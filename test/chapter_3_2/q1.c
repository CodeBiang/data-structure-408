#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 5

struct {
    int front, rear;
    int tag;  // 0 空, 1 满
    int data[MAX_SIZE];
} queue = {0, 0, 0, {0}};

static bool enqueue(int elem) {
    if (queue.front == queue.rear && queue.tag == 1) {
        return false;
    }
    int newrear = (queue.rear + 1) % MAX_SIZE;

    if (newrear == queue.front) {
        queue.tag = 1;
    }

    queue.data[queue.rear] = elem;
    queue.rear = newrear;
    return true;
}

static bool dequeue(int* elem) {
    if (queue.front == queue.rear && queue.tag == 0) {
        return false;
    }

    int newfront = (queue.front + 1) % MAX_SIZE;

    if (newfront == queue.front) {
        queue.tag = 0;
    }

    *elem = queue.data[queue.front];
    queue.front = newfront;
    return true;
}

int main() {
    printf("%d\n", enqueue(1));
    printf("%d\n", enqueue(1));
    printf("%d\n", enqueue(1));
    printf("%d\n", enqueue(1));
    printf("%d\n", enqueue(1));
    printf("%d\n", enqueue(1));
    int temp;
    dequeue(&temp);
    return 0;
}
