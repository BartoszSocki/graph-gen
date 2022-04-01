#include "dijkstra.h"


DijkstraResult * dijkstra(Graph * graph, int source)
{
    int no_vertcies = graph->rows * graph->cols;

    DijkstraResult * result = malloc(sizeof(*result));
    result->dist = malloc(no_vertcies * sizeof(*result->dist));
    result->pred = malloc(no_vertcies * sizeof(*result->pred));
    result->source = source;
    result->no_verticies = no_vertcies;

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


void dijkstra_print_result(DijkstraResult * res)
{
    printf("Result of dijkstra:\n");
    for(int i =0; i < res->no_verticies; i++ )
    {
        printf("Vertex %d\n", i);
        printf("\tDistance from %d: %d\n", res->source, res->pred[i] );
        printf("\tPredecessor: %d\n", res->pred[i] );
    }
}

void dijkstra_print_path(DijkstraResult * res, int to)
{

    if(res->pred[to] == -1)
    {
        printf("There is no path between %d and %d.\n", res->source, to);
        return;
    }


    printf("Path from %d to %d\n", res->source, to);

    int path_size = 10;
    int * path = malloc(sizeof(*path) * path_size);
     
    //start from the end of the path 
    int i = 0;
    path[i] = to;

    while(path[i] != -1)
    {

        i = i+1; 
        //get previous vertex of the last inserted element in the path 
        path[i] = res->pred[path[i-1]];

        if(path_size-1 == i)
        {
            path_size*=2;
            path = realloc(path, path_size * sizeof(*path));
        }
    }
    
    //print path
    i--;
    while(--i >= 0)
    {
        printf("%d\t->\t%d\n", path[i+1], path[i]);
    }


    free(path);
}
