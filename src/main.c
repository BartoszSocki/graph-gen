#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include "graph.h"
#include "bfs.h"
#include "dijkstra.h"

static void error_many_options(char* option_name) {
	fprintf(stderr, "graphalgo: %s passed multiple times\n", option_name);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
	int opt, long_index = 0;

	int is_gen = 0;
	int is_seed = 0;
	int is_rows = 0;
	int is_cols = 0;
	int is_min = 0;
	int is_max = 0;
	int is_bfs = 0;
	int is_dijkstra = 0;
	int is_vert1 = 0;
	int is_vert2 = 0;

	long seed;
	int rows;
	int cols;
	double min;
	double max;
	int vert1;
	int vert2;

	static struct option long_options[] = {
		{"generate"	, optional_argument, 0, 'g'},
		{"rows"		, optional_argument, 0, 'r'},
		{"cols"		, optional_argument, 0, 'c'},
		{"min"		, optional_argument, 0, 'n'},
		{"max"		, optional_argument, 0, 'x'},
		{"seed"		, optional_argument, 0, 's'},
		{"bfs"		, optional_argument, 0, 'b'},
		{"dijkstra"	, optional_argument, 0, 'd'},
		{"vert1"	, optional_argument, 0, '1'},
		{"vert2"	, optional_argument, 0, '2'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "bdgs:r:c:n:x:b:1:2:", long_options, &long_index)) != -1) {
		switch (opt) {
			case 'g':
				if (is_gen == 1)
					error_many_options("-g, --generate");
				is_gen = 1;
				break;
			case 's':
				if (is_seed == 1)
					error_many_options("-s, --seed");
				is_seed = 1;
				seed = atol(optarg);
				break;
			case 'r':
				if (is_rows == 1)
					error_many_options("-r, --rows");
				is_rows = 1;
				rows = atoi(optarg);
				break;
			case 'c':
				if (is_cols == 1)
					error_many_options("-c, --cols");
				is_cols = 1;
				cols = atoi(optarg);
				break;
			case 'n':
				if (is_min == 1)
					error_many_options("-n, --min");
				is_min = 1;
				min = atof(optarg);
				break;
			case 'x':
				if (is_max == 1)
					error_many_options("-x, --max");
				is_max = 1;
				max = atof(optarg);
				break;
			case 'b':
				if (is_bfs == 1)
					error_many_options("-b, --bfs");
				is_bfs = 1;
				break;
			case 'd':
				if (is_dijkstra == 1)
					error_many_options("-d, --dijkstra");
				is_dijkstra = 1;
				break;
			case '1':
				if (is_vert1 == 1)
					error_many_options("-1, --vert1");
				is_vert1 = 1;
				vert1 = atoi(optarg);
				break;
			case '2':
				if (is_vert2 == 1)
					error_many_options("-2, --vert2");
				is_vert2 = 1;
				vert2 = atoi(optarg);
				break;
			default:
				fprintf(stderr, "graphalgo: invalid flag\n");
				exit(EXIT_FAILURE);
				break;
		}
	}

	/* printf("%d %d %d\n", is_dijkstra, is_vert1, is_vert2); */
	if (is_seed == 0)
		seed = time(NULL);

	int is_generation_selected = is_gen && is_rows && is_cols && is_min && is_max;

	if (is_generation_selected) {
		if (rows < 1 || cols < 1) {
			fprintf(stderr, "graphalgo: rows or cols cannot be smaller than 1\n");
			exit(EXIT_FAILURE);
		} else if (min > max) {
			fprintf(stderr, "graphalgo: min cannot be greater than max\n");
			exit(EXIT_FAILURE);
		}

		Graph *graph = graph_generate_from_seed(rows, cols, min, max, seed);
		graph_print_to_stdout(graph);
		graph_free(graph);

	} else if (is_bfs && is_vert1) {
		Graph *graph = graph_read_from_stdin();
		if (graph == NULL) {
			fprintf(stderr, "graphalgo: invalid graph\n");
			exit(EXIT_FAILURE);
		} else if (vert1 < 0 || vert1 >= graph->cols * graph->rows) {
			fprintf(stderr, "graphalgo: invalid vertex index\n");
			exit(EXIT_FAILURE);
		}
		bfs_print_result(bfs(graph, vert1));

	} else if (is_dijkstra && is_vert1 && is_vert2) {
		Graph * graph = graph_read_from_stdin();
		if(graph==NULL)
		{
			fprintf(stderr, "graphalgo: invalid graph\n");
			exit(EXIT_FAILURE);
		}
		if(vert1 >= graph->cols * graph->rows || vert2 >= graph->cols * graph->rows)
		{
			fprintf(stderr, "graphalgo: dijkstra: the specified verticies are not the part of the graph.\n");
			exit(EXIT_FAILURE);
		}
		DijkstraResult * result = dijkstra(graph, vert1);
		dijkstra_print_result(result);
		dijkstra_print_path(result, vert2);
		dijkstra_result_free(result);

	} else {
		fprintf(stderr, "graphalgo: not enough options passed\n");
		exit(EXIT_FAILURE);
	}
}
