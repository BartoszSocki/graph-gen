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
    VertexPriorityQueue * pr = vertex_priority_queue_initalize(10);

    for(int i = 9; i != -1; i--)
    {
        vertex_priority_queue_add(pr, test_vertex(i, DBL_MAX));
    }

    vertex_priority_queue_update(pr, 5, 13);
    vertex_priority_queue_poll(pr);
    vertex_priority_queue_update(pr, 1, 96);
    vertex_priority_queue_update(pr, 2, 13);

  
    while(pr->size != 0)
    {
        QueuedVertex * p = vertex_priority_queue_poll(pr);
        printf("%d - %g\n", p->index, p->dist);

        free(p);
    }


    vertex_priority_queue_free(pr);
    return 0;
}
