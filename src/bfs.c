#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "bfs.h"
#include "graph.h"

void bfs_print_result(BFSResult *result) {
	if (result == NULL) {
		puts("(null)");
		return;
	}

	puts("Vertex\tDistance");
	for (int i = 0; i < result->size; i++) {
		/* tylko jeżeli istnieje */
		if (result->verticies[i] != -1 && result->visited[result->verticies[i]])
			printf("%d\t%lf\n", result->verticies[i], result->distance[i]);
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
	result->verticies = calloc(graph_size, sizeof(*(result->verticies)));
	memset(result->verticies, -1, graph_size * sizeof(*(result->verticies)));

	result->distance = calloc(graph_size, sizeof(*(result->distance)));
	result->size = graph_size;

	if (result->verticies == NULL || result->distance == NULL || result->visited == NULL) {
		bfs_result_free(result);
		return NULL;
	}

	/* kolejka jest zaimplementowana, poprzez tablice o stałej wielkości i dwa wskaźniki, */
	/* ponieważ ilość elementów jest stała i znana */
	int beg_index = 0;
	int end_index = 1;

	/* dodanie pierwszego wierzchołka do kolejki */
	result->verticies[0] = start_vertex;
	result->visited[start_vertex] = true;

	while (beg_index < end_index) {
		int curr_vertex = result->verticies[beg_index++];
		Edge* edges = graph->edges[curr_vertex];
		/* dla wszystkich sąsiadów */
		while (edges != NULL) {
			if (!result->visited[edges->end_vertex]) {
				/* dodaj nieodwiedzony wierzchołek do kolejki */
				result->verticies[end_index] = edges->end_vertex;
				/* dystans dla tego wierzchołka, to dystans jego poprzednika + 1 */
				result->distance[end_index] = result->distance[beg_index - 1] + 1;
				end_index++;
				result->visited[edges->end_vertex] = true;
			}
			edges = edges->next;
		}
	}
	return result;
}

void bfs_result_free(BFSResult* result) {
	if (result == NULL)
		return;
	free(result->visited);
	free(result->distance);
	free(result->verticies);
	free(result);
 }
