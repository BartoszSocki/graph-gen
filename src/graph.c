#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "graph.h"

static double _uniformRandom(double from, double to) {
	return from + ((double) rand() * (to - from)) / RAND_MAX;
}

static int _xyToIndex(Graph* graph, int row, int col) {
	return row * graph->cols + col;
}

static void _vertexNodeAppend(VertexNode **list, VertexNode *elem) {
    VertexNode *dummy = *list;
    if (!dummy) {
		*list = elem;
		return;
	}

    while (dummy && dummy->next)
        dummy = dummy->next;

    dummy->next = elem;
}

static void _graphAddEdgeBetweenTwoVertexes(Graph *graph, size_t index_a, size_t index_b, double weight) {
	if (!graph || !graph->adj)
		return;
	if (index_a >= graph->rows * graph->cols || index_b >= graph->rows * graph->cols)
		return;

	VertexNode **a = &(graph->adj[index_a]);
	VertexNode **b = &(graph->adj[index_b]);

	_vertexNodeAppend(a, vertexNodeInit(index_b, weight, NULL));
	_vertexNodeAppend(b, vertexNodeInit(index_a, weight, NULL));
}

static void _graphGenerateAdjListCardinal(Graph *graph) {
    if (!graph)
        return;

    for (int i = 0; i < graph->rows - 1; i++) {
        for (int j = 0; j < graph->cols; j++) {
            double rand_val = _uniformRandom(graph->from, graph->to);
			_graphAddEdgeBetweenTwoVertexes(graph, _xyToIndex(graph, i, j), _xyToIndex(graph, i + 1, j), rand_val);
        }
    }

    for (int i = 0; i < graph->rows; i++) {
        for (int j = 0; j < graph->cols - 1; j++) {
            double rand_val = _uniformRandom(graph->from, graph->to);
			_graphAddEdgeBetweenTwoVertexes(graph, _xyToIndex(graph, i, j), _xyToIndex(graph, i, j + 1), rand_val);
        }
    }
}

VertexNode* vertexNodeInit(unsigned int adj_node, double weight, VertexNode *next) {
    VertexNode* node = malloc(sizeof(*node));
    if (!node)
        return NULL;

	node->adj_node = adj_node;
	node->weight = weight;
	node->next = next;

	return node;
}

Graph* graphGenerateFromSeed(size_t rows, size_t cols, double from, double to, long seed) {
	Graph* graph = malloc(sizeof(*graph));

    if (!graph)
        return NULL;

    graph->adj = calloc(rows * cols, sizeof(*graph->adj));
    if (!graph->adj)
        return NULL;

    graph->rows = rows;
    graph->cols = cols;
    graph->from = from;
	graph->to = to;

    srand(seed);
    _graphGenerateAdjListCardinal(graph);

    return graph;
}

Graph* graphReadFromStdin() {
	Graph *graph = malloc(sizeof(*graph));
	FILE *in = stdin;
	char *buff = NULL;
	size_t buff_size;

	if (getline(&buff, &buff_size, in) <= 0)
		exit(EXIT_FAILURE);

	sscanf(buff, "%zd %zd", &graph->rows, &graph->cols);

    graph->adj = calloc(graph->rows * graph->cols, sizeof(*graph->adj));
    if (!graph->adj)
        return NULL;

	for (int i = 0; i < graph->rows * graph->cols; i++) {
		if (getline(&buff, &buff_size, in) <= 0)
			exit(EXIT_FAILURE);

		int curr_len = 0;
		int buff_len = strlen(buff);
		char *buff_ptr = buff;
		while (curr_len < buff_len) {
			int adj_vertex, n;
			double weight;
			sscanf(buff_ptr, "%d : %lf %n", &adj_vertex, &weight, &n);

			_graphAddEdgeBetweenTwoVertexes(graph, i, adj_vertex, weight);

			buff_ptr += n;
			curr_len += n;
		}
	}

	free(buff);
	return graph;
}

void graphPrintToStdout(Graph *graph) {
    if (!graph) {
        puts("0 0");
        return;
    }

    printf("%zu %zu\n", graph->rows, graph->cols);
    for (int i = 0; i < graph->rows; i++) {
        for (int j = 0; j < graph->cols; j++) {
            VertexNode* dummy = graph->adj[_xyToIndex(graph, i, j)];
			/* printf("%p", dummy); */
			printf("\t");
            while (dummy) {
                printf("%u :%lf  ", dummy->adj_node, dummy->weight);
                dummy = dummy->next;
            }
            printf("\n");
        }
    }
}
