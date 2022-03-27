#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>

typedef struct Edge_t {
	struct Edge_t* next;
	int end_vertex;
	double weight;
} Edge;

typedef struct {
	/* tablica list krawędzi, gdzie edges[i] jest listą krawędzi z wierzchołka i */
	Edge** edges;
	size_t rows;
	size_t cols;
} Graph;

Edge* edge_init(int connected_vertex, double weight, Edge* next);
void edge_free(Edge* edge);

Graph* graph_generate_from_seed(int rows, int cols, double from, double to, long seed);

/* zwraca błąd przy niepoprawnym formacie */
int graph_read_from_stdin(Graph* graph);
void graph_print_to_stdout(Graph* graph);
int graph_xy_to_index(Graph* graph, int row, int col);
void graph_free(Graph* graph);

#endif
