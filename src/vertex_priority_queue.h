#ifndef _PRIORITY_QUEUE
#define _PRIORITY_QUEUE

typedef struct{
    int index;
    double dist;
} QueuedVertex;

typedef struct{

    //the size of queue
    int capacity; 
    //number of elements in the queue
    int size;
    //array which maps vertex index to index in verticies
    // vertex_indexes[0] = i  
    // verticies[i] -> QueuedVertex.index = 0   
    int * verticies_indexes;
    
    QueuedVertex ** verticies;
} VertexPriorityQueue;

VertexPriorityQueue * vertex_priority_queue_initalize(int number_of_verticies);
void vertex_priority_queue_free(VertexPriorityQueue * pr);


void vertex_priority_queue_add(VertexPriorityQueue * pr, QueuedVertex * item);
void vertex_priority_queue_add_empty(VertexPriorityQueue * pr, int index);
QueuedVertex *  vertex_priority_queue_poll(VertexPriorityQueue * pr);
void vertex_priority_queue_update(VertexPriorityQueue * pr, int index, double new_dist);



#endif