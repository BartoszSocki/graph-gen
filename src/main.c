#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include "graph.h"

static void _error_many_options(char* option_name) {
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
				if (is_gen == 1)
					_error_many_options("-g, --generate");
				is_gen = 1;
				break;
			case 's':
				if (is_seed == 1)
					_error_many_options("-s, --seed");
				is_seed = 1;
				seed = atol(optarg);
				break;
			case 'r':
				if (is_rows == 1)
					_error_many_options("-r, --rows");
				is_rows = 1;
				rows = atoi(optarg);
				break;
			case 'c':
				if (is_cols == 1)
					_error_many_options("-c, --cols");
				is_cols = 1;
				cols = atoi(optarg);
				break;
			case 'n':
				if (is_min == 1)
					_error_many_options("-n, --min");
				is_min = 1;
				min = atof(optarg);
				break;
			case 'x':
				if (is_max == 1)
					_error_many_options("-x, --max");
				is_max = 1;
				max = atof(optarg);
				break;
			default:
				fprintf(stderr, "graphalgo: invalid flag\n");
				exit(EXIT_FAILURE);
				break;
		}
	}

	if (is_seed == 0)
		seed = time(NULL);

	if (is_gen && is_rows && is_cols && is_min && is_max) {
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
	} else {
		fprintf(stderr, "graphalgo: not enough options passed\n");
		exit(EXIT_FAILURE);
	}
}
