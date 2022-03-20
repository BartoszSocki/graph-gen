#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

static double _uniform_random(double min, double max) {
	return min + ((double) rand() * (max - min)) / RAND_MAX;
}

static void _edge_node_append(EdgeNode **list, EdgeNode *elem) {
    EdgeNode *dummy = *list;
    if (!dummy) {
		*list = elem;
		return;
	}

    while (dummy && dummy->next)
        dummy = dummy->next;

    dummy->next = elem;
}

static void _graph_add_directed_edge(Graph *graph, size_t beg_vertex, size_t end_vertex, double weight) {
	if (!graph || !graph->edges)
		return;
	/* cannot add edge when edge index is greater than graph size */
	if (beg_vertex >= graph->rows * graph->cols || end_vertex >= graph->rows * graph->cols)
		return;

	EdgeNode **a = &(graph->edges[beg_vertex]);

	_edge_node_append(a, edge_node_init(end_vertex, weight, NULL));
}

/* w kierunkach kardynalnych */
/* do tego skierowane w dwie strony */
static void _graph_generate_bidirectional_edges_cardinal(Graph *graph, double min, double max) {
    if (!graph)
        return;

	if (min > max)
		return;

    for (int i = 0; i < graph->rows - 1; i++) {
        for (int j = 0; j < graph->cols; j++) {
            double weight = _uniform_random(min, max);
			_graph_add_directed_edge(graph, graph_xy_to_index(graph, i, j), graph_xy_to_index(graph, i + 1, j), weight);
			_graph_add_directed_edge(graph, graph_xy_to_index(graph, i + 1, j), graph_xy_to_index(graph, i, j), weight);
        }
    }

    for (int i = 0; i < graph->rows; i++) {
        for (int j = 0; j < graph->cols - 1; j++) {
            double weight = _uniform_random(min, max);
			_graph_add_directed_edge(graph, graph_xy_to_index(graph, i, j), graph_xy_to_index(graph, i, j + 1), weight);
			_graph_add_directed_edge(graph, graph_xy_to_index(graph, i, j + 1), graph_xy_to_index(graph, i, j), weight);
        }
    }
}

/* zamienia "współrzędne x, y" na indeks w tablicy opisującej krawędzie grafu */
int graph_xy_to_index(Graph* graph, int row, int col) {
	return row * graph->cols + col;
}

EdgeNode* edge_node_init(int end_vertex, double weight, EdgeNode *next) {
    EdgeNode* edge = malloc(sizeof(*edge));
    if (!edge)
        return NULL;

	edge->end_vertex = end_vertex;
	edge->weight = weight;
	edge->next = next;

	return edge;
}

Graph* graph_generate_from_seed(int rows, int cols, double min, double max, long seed) {
	Graph* graph = malloc(sizeof(*graph));

    if (!graph)
        return NULL;

    graph->edges = calloc(rows * cols, sizeof(*graph->edges));
    if (!graph->edges)
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
    _graph_generate_bidirectional_edges_cardinal(graph, min, max);

    return graph;
}

/* TODO: dodaj opisy dla błędów formatu */
Graph* graph_read_from_stdin() {
	Graph *graph = malloc(sizeof(*graph));
	if (!graph)
		return NULL;

	FILE *in = stdin;
	char *buff = NULL;
	size_t _buff_size;

	if (getline(&buff, &_buff_size, in) <= 0)
		exit(EXIT_FAILURE);

	int rows, cols;
	if (sscanf(buff, "%d %d", &rows, &cols) != 2)
		exit(EXIT_FAILURE);

	if (rows <= 0 || cols <= 0)
		return NULL;

	graph->rows = rows;
	graph->cols = cols;
    graph->edges = calloc(graph->rows * graph->cols, sizeof(*graph->edges));
    if (!graph->edges) {
		graph_free(graph);
        return NULL;
	}

	for (int i = 0; i < graph->rows * graph->cols; i++) {
		if (getline(&buff, &_buff_size, in) <= 0) {
			graph_free(graph);
			exit(EXIT_FAILURE);
		}

		/* indeks w lini pobranej za pomocą getline */
		int line_index = 0;
		/* długość lini */
		int buff_len = strlen(buff);
		char *buff_ptr = buff;
		while (line_index < buff_len) {
			/* n jest ilością pobranych znaków przez sscanf */
			int end_vertex, n;
			double weight;
			sscanf(buff_ptr, "%d : %lf %n", &end_vertex, &weight, &n);

			_graph_add_directed_edge(graph, i, end_vertex, weight);

			buff_ptr += n;
			line_index += n;
		}
	}

	free(buff);
	return graph;
}

void graph_print_to_stdout(Graph *graph) {
    if (!graph) {
		/* rozwiązanie tymczasowe */
        puts("0 0");
        return;
    }

    printf("%zu %zu\n", graph->rows, graph->cols);
    for (int i = 0; i < graph->rows; i++) {
        for (int j = 0; j < graph->cols; j++) {
            EdgeNode* dummy = graph->edges[graph_xy_to_index(graph, i, j)];
			printf("\t");
            while (dummy) {
                printf("%u :%lf  ", dummy->end_vertex, dummy->weight);
                dummy = dummy->next;
            }
            printf("\n");
        }
    }
}

void edge_node_free(EdgeNode* edge) {
	EdgeNode *prev;
	while (edge) {
		prev = edge;
		prev->next = NULL;
		edge = edge->next;
		free(prev);
	}
	free(edge);
}

void graph_free(Graph* graph) {
	if (!graph)
		return;

	if (!graph->edges) {
		free(graph);
		return;
	}

	for (int i = 0; i < graph->rows * graph->cols; i++)
		edge_node_free(graph->edges[i]);

	free(graph->edges);
	free(graph);
	graph = NULL;
}
