#include <stdio.h>
#include <stdlib.h>
#include "float.h"
#include "vertex_priority_queue.h"


QueuedVertex * test_vertex(int index, double dist)
{
    QueuedVertex * q = malloc(sizeof(QueuedVertex));
    q->index = index;
    q->dist = dist;

    return q;
}


int main(int argc, char * argv[])
{
    VertexPriorityQueue * pr = vertex_priority_queue_initalize(100);

    for(int i = 99; i != -1; i--)
    {
        vertex_priority_queue_add(pr, test_vertex(i, DBL_MAX));
    }

    for(int i = 99; i != -1; i--)
    {
        vertex_priority_queue_update(pr, i, 99-i);
    }


  
    while(pr->size != 0)
    {
        QueuedVertex * p = vertex_priority_queue_poll(pr);
        printf("Vertex(index=%d, dist=%g)\n", p->index, p->dist);

        free(p);
    }


    vertex_priority_queue_free(pr);
    return 0;
}
