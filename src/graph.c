#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

static double uniform_random(double min, double max) {
	return min + ((double) rand() * (max - min)) / RAND_MAX;
}

/* zwraca 0 jak udało się dodać krawędź */
static int graph_add_directed_edge(Graph *graph, size_t beg_vertex, size_t end_vertex, double weight) {
	if (graph == NULL || graph->edges == NULL)
		return 1;
	/* indeks wierzchołka jest większy niż ilość wierzchołków w grafie */
	if (beg_vertex >= graph->rows * graph->cols || end_vertex >= graph->rows * graph->cols)
		return 1;

	Edge* new_edge = edge_init(end_vertex, weight, graph->edges[beg_vertex]);
	if (new_edge == NULL)
		return 1;

	graph->edges[beg_vertex] = new_edge;
	return 0;
}

/* w kierunkach kardynalnych */
/* do tego skierowane w dwie strony */
static int graph_generate_bidirectional_edges_cardinal(Graph *graph, double min, double max) {
    if (graph == NULL)
        return 1;

	if (min > max)
		return 1;

	/* łączenie wierzchołków, "górny z dolnym" */
    for (int i = 0; i < graph->rows - 1; i++) {
        for (int j = 0; j < graph->cols; j++) {
            double weight = uniform_random(min, max);
			int res1 = graph_add_directed_edge(graph, graph_xy_to_index(graph, i, j), graph_xy_to_index(graph, i + 1, j), weight);
			int res2 = graph_add_directed_edge(graph, graph_xy_to_index(graph, i + 1, j), graph_xy_to_index(graph, i, j), weight);
			if (res1 || res2) {
				/* printf("index out of bounds\n"); */
				return 1;
			}
        }
    }

	/* łączenie wierzchołków, "lewy z prawym" */
    for (int i = 0; i < graph->rows; i++) {
        for (int j = 0; j < graph->cols - 1; j++) {
            double weight = uniform_random(min, max);
			int res1 = graph_add_directed_edge(graph, graph_xy_to_index(graph, i, j), graph_xy_to_index(graph, i, j + 1), weight);
			int res2 = graph_add_directed_edge(graph, graph_xy_to_index(graph, i, j + 1), graph_xy_to_index(graph, i, j), weight);
			if (res1 || res2) {
				/* printf("index out of bounds\n"); */
				return 1;
			}
        }
    }
	return 0;
}

/* zamienia "współrzędne x, y" na indeks wierzchołka */
int graph_xy_to_index(Graph* graph, int row, int col) {
	return row * graph->cols + col;
}

Edge* edge_init(int end_vertex, double weight, Edge *next) {
    Edge* edge = malloc(sizeof(*edge));
    if (edge == NULL)
        return NULL;

	edge->end_vertex = end_vertex;
	edge->weight = weight;
	edge->next = next;

	return edge;
}

Graph* graph_generate_from_seed(int rows, int cols, double min, double max, long seed) {
	Graph* graph = malloc(sizeof(*graph));

    if (graph == NULL)
        return NULL;

    graph->edges = calloc(rows * cols, sizeof(*graph->edges));
    if (graph->edges == NULL)
        return NULL;

	if (rows <= 0 || cols <= 0) {
		graph_free(graph);
		return NULL;
	}

	if (min > max) {
		graph_free(graph);
		return NULL;
	}

    graph->rows = rows;
    graph->cols = cols;

    srand(seed);
	int res = graph_generate_bidirectional_edges_cardinal(graph, min, max);
	if (res) {
		/* puts("error occured during generation of graph edges"); */
		graph_free(graph);
		return NULL;
	}

    return graph;
}

Graph* graph_read_from_stdin() {
	Graph *graph = malloc(sizeof(*graph));
	if (graph == NULL)
		return NULL;

	char *line = NULL;
	/* ile bajtów bufora zaalokowano */
	size_t line_size;

	if (getline(&line, &line_size, stdin) < 1) {
		fprintf(stderr, "graphalgo: unexpected EOF at first line\n");
		graph_free(graph);
		exit(EXIT_FAILURE);
	}

	int rows, cols;
	/* nie podano wszystkich wymiarów grafu */
	if (sscanf(line, "%d %d", &rows, &cols) != 2) {
		fprintf(stderr, "graphalgo: invalid number of graph dimensions\n");
		graph_free(graph);
		exit(EXIT_FAILURE);
	}


	if (rows <= 0 || cols <= 0) {
		fprintf(stderr, "graphalgo: invalid values of graph dimensions\n");
		graph_free(graph);
		exit(EXIT_FAILURE);
	}

	graph->rows = rows;
	graph->cols = cols;
    graph->edges = calloc(graph->rows * graph->cols, sizeof(*graph->edges));
    if (graph->edges == NULL) {
		graph_free(graph);
        return NULL;
	}

	for (int i = 0; i < graph->rows * graph->cols; i++) {
		if (getline(&line, &line_size, stdin) < 1) {
			fprintf(stderr, "graphalgo: file incomplete\n");
			graph_free(graph);
			exit(EXIT_FAILURE);
		}

		int index = 0;
		int line_len = strlen(line);

		while (index < line_len) {
			int end_vertex, n;
			double weight;
			int assigned_values = sscanf(line + index, "%d : %lf %n", &end_vertex, &weight, &n);

			/* niekompletny opis krawędzi */
			if (assigned_values == 1) {
				fprintf(stderr, "graphalgo: file incomplete\n");
				graph_free(graph);
				exit(EXIT_FAILURE);
			}
				
			/* nie podano krawędzi */
			if (assigned_values < 1)
				break;

			/* wierzchołek poza grafem */
			if (graph_add_directed_edge(graph, i, end_vertex, weight) != 0) {
				fprintf(stderr, "graphalgo: invalid vertex index\n");
				graph_free(graph);
				exit(EXIT_FAILURE);
			}
			index += n;
		}
	}

	free(line);
	return graph;
}

void graph_print_to_stdout(Graph *graph) {
    if (graph == NULL) {
        puts("(null)");
        return;
    }

    printf("%zu %zu\n", graph->rows, graph->cols);
    for (int i = 0; i < graph->rows; i++) {
        for (int j = 0; j < graph->cols; j++) {
            Edge* dummy = graph->edges[graph_xy_to_index(graph, i, j)];
			printf("\t");
            while (dummy) {
                printf("%u :%lf  ", dummy->end_vertex, dummy->weight);
                dummy = dummy->next;
            }
            printf("\n");
        }
    }
}

void edge_free(Edge* edge) {
	Edge *prev;
	while (edge) {
		prev = edge;
		prev->next = NULL;
		edge = edge->next;
		free(prev);
	}
	free(edge);
}

void graph_free(Graph* graph) {
	if (graph == NULL)
		return;

	if (graph->edges == NULL) {
		free(graph);
		return;
	}

	for (int i = 0; i < graph->rows * graph->cols; i++)
		edge_free(graph->edges[i]);

	free(graph->edges);
	free(graph);
	graph = NULL;
}
