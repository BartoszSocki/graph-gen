#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <vertex_priority_queue.h>


/*
    RETURN CODE 0 MEANS THAT TEST PASSED
    ANY OTHER CODE MEANS THAT TEST FAILED
*/

int main(int argc, char * argv[])
{

    int no_vertex=1000;
    VertexPriorityQueue * pr = vertex_priority_queue_initalize(no_vertex);

    //create verticies
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
    //check if verticies are pulled in correct order
    for(int i = 0; i < no_vertex;i++)
    {
        QueuedVertex * min_vert = vertex_priority_queue_poll(pr);
        if(min_vert->index != no_vertex-1-i)
            return 0;
    }

    //clean up
    for(int i =0; i < no_vertex;i++)
    {
        free(q[i]);
    }
    vertex_priority_queue_free(pr);


    return 1;
}
