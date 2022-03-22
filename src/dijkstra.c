#include "dijkstra.h"


DijkstraResult * dijkstra(Graph * graph, int source)
{
    int no_vertcies = graph->rows * graph->cols;

    DijkstraResult * result = malloc(sizeof(*result));
    result->dist = malloc(no_vertcies * sizeof(*result->dist));
    result->pred = malloc(no_vertcies * sizeof(*result->pred));

    //set default values
    //all vertices have inf distance and predecessor is with -1 index
    VertexPriorityQueue * vpq = vertex_priority_queue_initalize(no_vertcies);
    for(int i =0; i < no_vertcies; i++)
    {
        vertex_priority_queue_add_empty(vpq, i);
        result->pred[i] = -1;
    }

    //update priority of source vertex so its picked first
    vertex_priority_queue_update(vpq, source, 0);
    while(vpq->size != 0)
    {

        QueuedVertex * u = vertex_priority_queue_poll(vpq);

        Edge * adj_vert = graph->edges[u->index];
        //iterate through all adjacent vertices 
        while(adj_vert != NULL)
        {
            //if vertex was already pulled from the queue, skip it
            if(vpq->verticies_indexes[adj_vert->end_vertex] == -1)
            {
                adj_vert = adj_vert->next;
                continue; 
            }
            
            double alt = u->dist + adj_vert->weight;
            if(alt < vpq->verticies[vpq->verticies_indexes[adj_vert->end_vertex]]->dist)
            {
                vertex_priority_queue_update(vpq, adj_vert->end_vertex, alt);
                result->pred[adj_vert->end_vertex] = u->index;
            }
         
            adj_vert = adj_vert->next;
        }

        result->dist[u->index] = u->dist;
        free(u);
    }
    
    vertex_priority_queue_free(vpq);
    return result;
}


void dijkstra_result_free(DijkstraResult * result)
{
    free(result->pred);
    free(result->dist);
    free(result);
}

