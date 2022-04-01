#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include "graph.h"
#include "bfs.h"
#include "dijkstra.h"

typedef struct {
	int is_help;
	int is_gen;
	int is_seed;
	int is_rows;
	int is_cols;
	int is_min;
	int is_max;
	int is_bfs;
	int is_dijkstra;
	int is_vert1;
	int is_vert2;

	long seed;
	int rows;
	int cols;
	double min;
	double max;
	int vert1;
	int vert2;
} ProgArgs;

static struct option long_options[] = {
	{"help"		, optional_argument, 0, 'h'},
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

static int is_gen_selected(ProgArgs* a) {
	if (a == NULL)
		return 0;
	int is_gen = a->is_gen && a->is_rows && a->is_cols && a->is_min && a->is_max;
	int is_other = a->is_bfs || a->is_dijkstra || a->is_vert1 || a->is_vert2;
	return is_gen && (!is_other);
}

static int is_bfs_selected(ProgArgs* a) {
	if (a == NULL)
		return 0;
	int is_bfs = a->is_bfs && a->is_vert1;
	int is_other = a->is_gen || a->is_rows || a->is_cols || a->is_min || a->is_max || a->is_dijkstra || a->is_vert2 || a->is_seed;
	return is_bfs && (!is_other);
}

static int is_dijkstra_selected(ProgArgs* a) {
	if (a == NULL)
		return 0;
	int is_dijkstra = a->is_dijkstra && a->is_vert1 && a->is_vert2;
	int is_other = a->is_gen || a->is_rows || a->is_cols || a->is_min || a->is_max || a->is_bfs || a->is_seed;
	return is_dijkstra && (!is_other);
}

static int is_help_selected(ProgArgs* a) {
	if (a == NULL)
		return 0;
	int is_other = a->is_gen || a->is_rows || a->is_cols || a->is_min || a->is_max || a->is_bfs || a->is_dijkstra || a->is_seed || a->is_vert1 || a->is_vert2;
	return a->is_help  && (!is_other);
}

static void print_help() {
	printf("graph generation:\n");
	printf("graphalgo --generate --rows=<ROWS> --cols=<COLS> --min=<MIN> --max=<MAX> [--seed=<SEED>]\n");
	printf("graphalgo -g -r<ROWS> -c<COLS> -n<MIN> -x<MAX> [-s<SEED>]\n");
	printf("bfs:\n");
	printf("graphalgo --bfs --vert1=<VERT1>\n");
	printf("graphalgo -b -1<VERT1>\n");
	printf("dijkstra:\n");
	printf("graphalgo --dijkstra --vert1=<VERT1> --vert2=<VERT2>\n");
	printf("graphalgo -d -1<VERT1> -2<VERT2>\n");
}

static void error_many_options(char* option_name) {
	fprintf(stderr, "graphalgo: %s passed multiple times\n", option_name);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
	int opt, long_index = 0;
	ProgArgs prog_args = {0};

	while ((opt = getopt_long(argc, argv, ":hbdgs:r:c:n:x:b:1:2:", long_options, &long_index)) != -1) {
		switch (opt) {
			case 'h':
				if (prog_args.is_help == 1)
					error_many_options("-h, --help");
				prog_args.is_help = 1;
				break;
			case 'g':
				if (prog_args.is_gen == 1)
					error_many_options("-g, --generate");
				prog_args.is_gen = 1;
				break;
			case 's':
				if (prog_args.is_seed == 1)
					error_many_options("-s, --seed");
				prog_args.is_seed = 1;
				prog_args.seed = atol(optarg);
				break;
			case 'r':
				if (prog_args.is_rows == 1)
					error_many_options("-r, --rows");
				prog_args.is_rows = 1;
				prog_args.rows = atoi(optarg);
				break;
			case 'c':
				if (prog_args.is_cols == 1)
					error_many_options("-c, --cols");
				prog_args.is_cols = 1;
				prog_args.cols = atoi(optarg);
				break;
			case 'n':
				if (prog_args.is_min == 1)
					error_many_options("-n, --min");
				prog_args.is_min = 1;
				prog_args.min = atof(optarg);
				break;
			case 'x':
				if (prog_args.is_max == 1)
					error_many_options("-x, --max");
				prog_args.is_max = 1;
				prog_args.max = atof(optarg);
				break;
			case 'b':
				if (prog_args.is_bfs == 1)
					error_many_options("-b, --bfs");
				prog_args.is_bfs = 1;
				break;
			case 'd':
				if (prog_args.is_dijkstra == 1)
					error_many_options("-d, --dijkstra");
				prog_args.is_dijkstra = 1;
				break;
			case '1':
				if (prog_args.is_vert1 == 1)
					error_many_options("-1, --vert1");
				prog_args.is_vert1 = 1;
				prog_args.vert1 = atoi(optarg);
				break;
			case '2':
				if (prog_args.is_vert2 == 1)
					error_many_options("-2, --vert2");
				prog_args.is_vert2 = 1;
				prog_args.vert2 = atoi(optarg);
				break;
			default:
				fprintf(stderr, "graphalgo: invalid flag\n");
				exit(EXIT_FAILURE);
				break;
		}
	}

	/* nie podano ziarna dla generatora */
	if (prog_args.is_seed == 0)
		prog_args.seed = time(NULL);

	if (is_help_selected(&prog_args)) {
		print_help();
		exit(EXIT_SUCCESS);

	} else if (is_gen_selected(&prog_args)) {
		if (prog_args.rows < 1 || prog_args.cols < 1) {
			fprintf(stderr, "graphalgo: rows and cols cannot be smaller than 1\n");
			exit(EXIT_FAILURE);
		} else if (prog_args.min > prog_args.max) {
			fprintf(stderr, "graphalgo: min cannot be greater than max\n");
			exit(EXIT_FAILURE);
		}

		Graph *graph = graph_generate_from_seed(prog_args.rows, prog_args.cols, prog_args.min, prog_args.max, prog_args.seed);
		graph_print_to_stdout(graph);
		graph_free(graph);

	} else if (is_bfs_selected(&prog_args)) {
		Graph* graph = malloc(sizeof(*graph));
		int did_fail = graph_read_from_stdin(graph);

		if (did_fail) {
			graph_free(graph);
			fprintf(stderr, "graphalgo: invalid graph\n");
			exit(EXIT_FAILURE);
		} 
		if (prog_args.vert1 < 0 || prog_args.vert1 >= graph->cols * graph->rows) {
			graph_free(graph);
			fprintf(stderr, "graphalgo: invalid vertex index\n");
			exit(EXIT_FAILURE);
		}

		BFSResult *result = bfs(graph, prog_args.vert1);
		bfs_print_result(result);
		bfs_result_free(result);
		graph_free(graph);

	} else if (is_dijkstra_selected(&prog_args)) {
		Graph * graph = malloc(sizeof(*graph));
		int did_fail = graph_read_from_stdin(graph);
		
		graph_print_to_stdout(graph);

		if(did_fail) {
			graph_free(graph);
			fprintf(stderr, "graphalgo: invalid graph\n");
			exit(EXIT_FAILURE);
		}
		if(prog_args.vert1 >= graph->cols * graph->rows || prog_args.vert2 >= graph->cols * graph->rows) {
			graph_free(graph);
			fprintf(stderr, "graphalgo: dijkstra: the specified verticies are not the part of the graph.\n");
			exit(EXIT_FAILURE);
		}

		DijkstraResult * result = dijkstra(graph, prog_args.vert1);
		dijkstra_print_result(result);
		dijkstra_print_path(result, prog_args.vert2);
		dijkstra_result_free(result);
		graph_free(graph);

	} else {
		fprintf(stderr, "graphalgo: unfamiliar combination of program options\n");
		exit(EXIT_FAILURE);
	}
}
