#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "vertex_priority_queue.h"




int main(int argc, char * argv[])
{
    VertexPriorityQueue * pr = vertex_priority_queue_initalize(10);

    //create verticies
    int no_vertex=100;
    QueuedVertex ** q;
    q = malloc(no_vertex * sizeof(*q));
    for(int i =0; i< no_vertex; i++)
    {
        q[i] = malloc(sizeof(*q[i]));
        q[i]->dist = DBL_MAX;
        q[i]->index = i;
    }

    //add verticies to the queue
    for(int i =0; i< no_vertex; i++)
        vertex_priority_queue_add(pr, q[i]);
    //update their distance
    for(int i =0; i< no_vertex; i++)
        vertex_priority_queue_update(pr, i, no_vertex-i-1);
    //poll until the queue is empty
    while(pr->size != 0)
    {
        QueuedVertex * a = vertex_priority_queue_poll(pr);
        printf("Vertex(index=%d, dist=%g)\n", a->index, a->dist);
    }

    //clean up
    for(int i =0; i < no_vertex;i++)
    {
        free(q[i]);
    }
    vertex_priority_queue_free(pr);


    return 0;
}
