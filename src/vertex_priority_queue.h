#ifndef _PRIORITY_QUEUE
#define _PRIORITY_QUEUE

typedef struct{
    int index;
    double dist;
} QueuedVertex;

typedef struct{

    int capacity; 
    int size;

    int * vertex_indexes;
    QueuedVertex ** verticies;
} VertexPriorityQueue;

VertexPriorityQueue * vertex_priority_queue_initalize(int number_of_verticies);

void vertex_priority_queue_free(VertexPriorityQueue * pr);

void vertex_priority_queue_add(VertexPriorityQueue * pr, QueuedVertex * item);
QueuedVertex *  vertex_priority_queue_poll(VertexPriorityQueue * pr);


void vertex_priority_queue_update(VertexPriorityQueue * pr, int index, double new_dist);



#endif