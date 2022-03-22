#include <dijkstra.h>
#include <graph.h>



int main(int argc, char * argv[])
{
    Graph * graph = graph_generate_from_seed(5, 1, 0, 99, 42);
    graph_print_to_stdout(graph);

    DijkstraResult * result_of_dijsktra = dijkstra(graph, 4);


    for(int i =0; i < graph->cols * graph->rows;i++)
    {
        printf("%d -> %d %g\n", i, result_of_dijsktra->pred[i], result_of_dijsktra->dist[i]);
    }

    dijkstra_result_free(result_of_dijsktra);
    
    graph_free(graph);
    return 0;
}