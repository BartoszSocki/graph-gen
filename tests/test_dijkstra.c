#include <dijkstra.h>
#include <graph.h>



int main(int argc, char * argv[])
{
    Graph * graph = graph_generate_from_seed(100, 1, 0, 99, 42);

    DijkstraResult * result_of_dijsktra = dijkstra(graph, 0);

    for(int i = 99; i >0; i--)
    {
        if(result_of_dijsktra->pred[i] != i-1)
            return 1;
    }

    dijkstra_print_result(result_of_dijsktra);
    dijkstra_print_path(result_of_dijsktra, 99);
    dijkstra_result_free(result_of_dijsktra);
    
    graph_free(graph);
    return 0;
}