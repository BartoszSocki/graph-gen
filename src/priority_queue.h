#ifndef _PRIORITY_QEUEUE
#define _PRIORITY_QEUEUE

typedef struct{

    int capacity; 
    int size;

    int * items;
} PriorityQueue;

PriorityQueue * priority_queue_initalize(int init_capacity);

void priority_queue_free(PriorityQueue * pr);
int priority_queue_add(PriorityQueue * pr, int item);
int priority_queue_poll(PriorityQueue * pr);

#endif