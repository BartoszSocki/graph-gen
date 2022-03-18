#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include "graph.h"

int main(int argc, char** argv) {
	int opt, long_index = 0;

	// przepraszam za to
	int is_gen = 0;
	int is_seed = 0;
	int is_rows = 0;
	int is_cols = 0;
	int is_min = 0;
	int is_max = 0;

	long seed;
	int rows;
	int cols;
	double min;
	double max;

	static struct option long_options[] = {
		{"generate"	, optional_argument, 0, 'g'},
		{"rows"		, optional_argument, 0, 'r'},
		{"cols"		, optional_argument, 0, 'c'},
		{"min"		, optional_argument, 0, 'n'},
		{"max"		, optional_argument, 0, 'x'},
		{"seed"		, optional_argument, 0, 's'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "gs:r:c:n:x:", long_options, &long_index)) != -1) {
		switch (opt) {
			case 'g':
				is_gen = 1;
				break;
			case 's':
				is_seed = 1;
				seed = atol(optarg);
				break;
			case 'r':
				is_rows = 1;
				rows = atoi(optarg);
				break;
			case 'c':
				is_cols = 1;
				cols = atoi(optarg);
				break;
			case 'n':
				is_min = 1;
				min = atof(optarg);
				break;
			case 'x':
				is_max = 1;
				max = atof(optarg);
				break;
			default:
				printf("invalid flag\n");
				break;
		}
	}

	if (is_seed == 0)
		seed = time(NULL);

	if (is_gen && is_rows && is_cols && is_min && is_max) {
		Graph *graph = graph_generate_from_seed(rows, cols, min, max, seed);
		graph_print_to_stdout(graph);
		graph_free(graph);
	} 

