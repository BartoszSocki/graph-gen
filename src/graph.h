#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>

typedef struct _VertexNode {
	struct _VertexNode* next;
	unsigned int adj_node;
	double weight;
} VertexNode;

typedef struct _Graph {
	VertexNode** adj;
	size_t rows;
	size_t cols;

	double from;
	double to;
} Graph;

VertexNode* vertexNodeInit(unsigned int adj, double weight, VertexNode* next);
Graph* graphGenerateFromSeed(size_t rows, size_t cols, double from, double to, long seed);
Graph* graphReadFromStdin();
void graphPrintToStdout(Graph* graph);

#endif
