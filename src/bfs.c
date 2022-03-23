#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "bfs.h"
#include "graph.h"

void bfs_print_result(BFSResult *result) {
	if (result == NULL) {
		puts("(null)");
	}

	for (int i = 0; i < result->size; i++) {
		if (result->visited[i])
			printf("%d %lf\n", result->verticies[i], result->weights[i]);
	}
}

BFSResult *bfs(Graph *graph, int start_vertex) {
	if (graph == NULL)
		return NULL;

	BFSResult *result = malloc(sizeof(*result));
	if (result == NULL)
		return NULL;

	size_t graph_size = graph->rows * graph->cols;
	if (start_vertex < 0 || start_vertex >= graph_size)
		return NULL;

	result->visited = calloc(graph_size, sizeof(*(result->visited)));
	result->verticies = malloc(graph_size * sizeof(*(result->verticies)));
	result->weights = malloc(graph_size * sizeof(*(result->weights)));
	result->size = graph_size;

	if (result->verticies == NULL || result->weights == NULL || result->visited == NULL) {
		free(result->verticies);
		free(result->weights);
		free(result->visited);
		free(result);
		return NULL;
	}

	/* dirty queue implementation, because size = const */
	int beg_index = 0;
	int end_index = 1;

	result->verticies[0] = start_vertex;
	result->weights[0] = 0;
	result->visited[0] = true;

	while (beg_index < end_index) {
		int curr_vertex = result->verticies[beg_index++];
		Edge* edges = graph->edges[curr_vertex];
		while (edges != NULL) {
			if (!result->visited[edges->end_vertex]) {
				result->verticies[end_index] = edges->end_vertex;
				result->weights[end_index] = result->weights[curr_vertex] + 1;
				end_index++;
				result->visited[edges->end_vertex] = true;
			}
			edges = edges->next;
		}
	}
	return result;
}
