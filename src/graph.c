#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "graph.h"

static double _uniformRandom(double from, double to) {
    double rand_val = (double)rand() / RAND_MAX;
    return from + rand_val * (to - from);
}

static int _xyToIndex(Graph* graph, int x, int y) {
    return y * graph->rows + x;
}

static VertexNode* _vertexNodeAppend(VertexNode* list, VertexNode* elem) {
    VertexNode* dummy = list;
    if (!list)
        return elem;

    while (dummy && dummy->next)
        dummy = dummy->next;

    dummy->next = elem;
    return list;
}

// this is stupid but I cannot find an easy way of generating undirected edges :/
static void _graphGenerateAdjListCardinal(Graph* graph) {
    if (!graph)
        return;

    VertexNode* left, right, top, bottom;
    for (int i = 0; i < graph->rows - 1; i++) {
        for (int j = 0; j < graph->cols; j++) {
            double rand_val = _uniformRandom(graph->from, graph->to);
            left = graph->adj[_xyToIndex(graph, i, j)];
            right = graph->adj[_xyToIndex(graph, i + 1, j)];

            _vertexNodeAppend(left, vertexNodeInit(_xyToIndex(graph, i + 1, j), rand_val, NULL));
            _vertexNodeAppend(right, vertexNodeInit(_xyToIndex(graph, i, j), rand_val, NULL));
        }
    }

    for (int i = 0; i < graph->rows; i++) {
        for (int j = 0; j < graph->cols - 1; j++) {
            double rand_val = _uniformRandom(graph->from, graph->to);
            top = graph->adj[_xyToIndex(graph, i, j)];
            bottom = graph->adj[_xyToIndex(graph, i, j + 1)];

            _vertexNodeAppend(top, vertexNodeInit(_xyToIndex(graph, i, j), rand_val, NULL));
            _vertexNodeAppend(bottom, vertexNodeInit(_xyToIndex(graph, i, j + 1), rand_val, NULL));
        }
    }
}

VertexNode* vertexNodeInit(unsigned int adj, double weight, VertexNode* next) {
    VertexNode* node = malloc(sizeof(*node));
    if (!node)
        return NULL;

    node->adj = adj;
	node->weight = weight;
	node->next = next;

	return node;
}

Graph* graphGenerateFromSeed(size_t rows, size_t cols, double from, double to, long seed) {
    if (!graph)
        return NULL;

    graph->adj = calloc(rows * cols, sizeof(*graph->adj));
    if (!graph->adj)
        return NULL;

    graph->rows = rows;
    graph->cols = cols;

    srand(seed);
    _graphGenerateAdjListCardinal(graph);

    return graph;
}

Graph* graphReadFromStdin() {
    /* ... */
}

void graphPrintToStdout(Graph* graph) {
    if (!graph) {
        puts("(null)");
        return;
    }

    printf("%d %d\n", graph->rows, graph->cols);
    for (int i = 0; i < graph->rows; i++) {
        for (int j = 0; i < graph->cols; j++) {
            VertexNode* dummy = graph->adj[_xyToIndex(graph, i, j)];
            while (dummy) {
                printf("%d :%lf  ", dummy->adj_node, dummy->weight);
                dummy = dummy->next;
            }
            printf("\n");
        }
    }
}
