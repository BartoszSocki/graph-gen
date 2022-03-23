#ifndef _BFS_H_
#define _BFS_H_

#include <stdlib.h>
#include <stdbool.h>
#include "graph.h"

typedef struct {
	bool *visited;
	int *verticies;
	double *weights;

	size_t size;
} BFSResult;

BFSResult *bfs(Graph* graph, int start_vertex);
void bfs_print_result(BFSResult *result);

#endif
