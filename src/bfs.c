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

	result->verticies = malloc(graph_size * sizeof(*(result->verticies)));
	result->weights = malloc(graph_size * sizeof(*(result->weights)));
	result->size = graph_size;
	bool *visited = calloc(graph_size, sizeof(*visited));
	int *queue = malloc(graph_size * sizeof(*queue));

	if (result->verticies == NULL || result->weights == NULL || visited == NULL || queue == NULL) {
		free(result->verticies);
		free(result->weights);
		free(result);
		free(visited);
		free(queue);
		return NULL;
	}

	/* dirty queue implementation, because size = const */
	int beg_index = 0;
	int end_index = 1;

	result->verticies[0] = start_vertex;
	result->weights[0] = 0;
	visited[0] = true;

	while (end_index < result->size) {
		int curr_vertex = result->verticies[beg_index++];
		Edge* edges = graph->edges[curr_vertex];
		while (edges != NULL) {
			if (!visited[edges->end_vertex]) {
				result->verticies[end_index] = edges->end_vertex;
				result->weights[end_index] = result->weights[curr_vertex] + 1;
				end_index++;
				visited[edges->end_vertex] = true;
			}
			edges = edges->next;
		}
	}
	return result;
}
