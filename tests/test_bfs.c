#include <stdio.h>
#include <stdlib.h>
#include "bfs.h"
#include "graph.h"

int main(int argc, char** argv) {
	/* when null is passed */
	BFSResult* result1 = bfs(NULL, 0);
	if (result1 != NULL) {
		bfs_result_free(result1);
		return EXIT_FAILURE;
	}

	/* vertex out of bounds */
	Graph* graph = graph_generate_from_seed(1, 1, 0.0, 1.0, 0);
	BFSResult* result2 = bfs(graph, 9);
	if (result2 != NULL) {
		bfs_result_free(result1);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
