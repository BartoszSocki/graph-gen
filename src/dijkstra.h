#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include <stdio.h>
#include "vertex_priority_queue.h"
#include <float.h>
#include "graph.h"

typedef struct{
    int * pred;
    double * dist;
} DijkstraResult;


DijkstraResult * dijkstra(Graph * graph, int source);
void dijkstra_result_free(DijkstraResult * result);
#endif