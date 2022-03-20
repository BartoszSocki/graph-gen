#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>

typedef struct _EdgeNode {
	struct _EdgeNode* next;
	int end_vertex;
	double weight;
} EdgeNode;

typedef struct _Graph {
	/* tablica list krawędzi, gdzie edges[i] jest listą krawędzi z wierzchołka i */
	EdgeNode** edges;
	size_t rows;
	size_t cols;
} Graph;

EdgeNode* edge_node_init(int connected_vertex, double weight, EdgeNode* next);
void edge_node_free(EdgeNode* edge);

Graph* graph_generate_from_seed(int rows, int cols, double from, double to, long seed);

/* zwraca błąd przy niepoprawnym formacie */
Graph* graph_read_from_stdin();
void graph_print_to_stdout(Graph* graph);
int graph_xy_to_index(Graph* graph, int row, int col);
void graph_free(Graph* graph);

#endif
