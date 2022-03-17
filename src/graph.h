#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>

typedef struct _EdgeNode {
	struct _EdgeNode* next;
	int end_vertex;
	double weight;
} EdgeNode;

typedef struct _Graph {
	EdgeNode** edges;
	size_t rows;
	size_t cols;
} Graph;

EdgeNode* edge_node_init(int connected_vertex, double weight, EdgeNode* next);
Graph* graph_generate_from_seed(size_t rows, size_t cols, double from, double to, long seed);
Graph* graph_read_from_stdin();
void graph_print_to_stdout(Graph* graph);

#endif
