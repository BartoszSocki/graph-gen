#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include <stdio.h>
#include "vertex_priority_queue.h"
#include <float.h>
#include "graph.h"

typedef struct{
    int * pred;
    double * dist;
    int source;
    int no_verticies;
} DijkstraResult;


DijkstraResult * dijkstra(Graph * graph, int source);
void dijkstra_result_free(DijkstraResult * result);

void dijkstra_print_result(DijkstraResult * res);

void dijkstra_print_path(DijkstraResult * res, int to);

#endif
