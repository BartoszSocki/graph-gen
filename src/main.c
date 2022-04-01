#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include "graph.h"
#include "bfs.h"
#include "dijkstra.h"

#define PROGRAM_ERROR(message) { fprintf(stderr, "%s: %s\n", argv[0], message); exit(EXIT_FAILURE); }

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

/* maski bitowe opcji */
typedef enum {
	INVALID = 0,
	IS_HELP = 1,
	IS_GEN = 1 << 1,
	IS_ROWS = 1 << 2,
	IS_COLS = 1 << 3,
	IS_MIN = 1 << 4,
	IS_MAX = 1 << 5,
	IS_SEED = 1 << 6,
	IS_BFS = 1 << 7,
	IS_DIJKSTRA = 1 << 8,
	IS_VERT1 = 1 << 9,
	IS_VERT2 = 1 << 10
} PROG_OPTION;

static struct option long_options[] = {
	{"help"		, no_argument, 0, 'h'},
	{"generate"	, no_argument, 0, 'g'},
	{"rows"		, required_argument, 0, 'r'},
	{"cols"		, required_argument, 0, 'c'},
	{"min"		, required_argument, 0, 'n'},
	{"max"		, required_argument, 0, 'x'},
	{"seed"		, required_argument, 0, 's'},
	{"bfs"		, no_argument, 0, 'b'},
	{"dijkstra"	, no_argument, 0, 'd'},
	{"vert1"	, required_argument, 0, '1'},
	{"vert2"	, required_argument, 0, '2'},
	{0, 0, 0, 0}
};

/* mapowanie: wartość z long_option -> maska bitowa opcji */
PROG_OPTION char_to_prog_option(int ch) {
	switch (ch) {
		case 'h': return IS_HELP;
		case 'g': return IS_GEN;
		case 'r': return IS_ROWS;
		case 'c': return IS_COLS;
		case 'n': return IS_MIN;
		case 'x': return IS_MAX;
		case 's': return IS_SEED;
		case 'b': return IS_BFS;
		case 'd': return IS_DIJKSTRA;
		case '1': return IS_VERT1;
		case '2': return IS_VERT2;
		default:  return INVALID;
	}
}

int main(int argc, char** argv) {
	int opt, long_index = 0;
	int prog_options = 0;

	long seed;
	int rows, cols;
	double min, max;
	int vert1, vert2;

	while ((opt = getopt_long(argc, argv, ":hbdgs:r:c:n:x:b:1:2:", long_options, &long_index)) != -1) {
		PROG_OPTION curr_option = char_to_prog_option(opt);

		/* sprawdź czy już była wcześniej zaznaczona */
		if ((prog_options & curr_option) != 0)
			PROGRAM_ERROR("duplicated option")

		/* dodanie opcji */
		prog_options |= curr_option;

		switch (opt) {
			case 's':
				seed = atol(optarg);
				break;
			case 'r':
				rows = atoi(optarg);
				break;
			case 'c':
				cols = atoi(optarg);
				break;
			case 'n':
				min = atof(optarg);
				break;
			case 'x':
				max = atof(optarg);
				break;
			case '1':
				vert1 = atoi(optarg);
				break;
			case '2':
				vert2 = atoi(optarg);
				break;
			case 'h':
			case 'g':
			case 'b':
			case 'd':
				break;
			default:
				PROGRAM_ERROR("cannot find an option, for help run: graphalgo --help")
				break;
		}
	}

	/* wszystkie kombinacje opcji obsługiwane przez program */
	const int SEED = IS_SEED;
	const int HELP = IS_HELP;
	const int GEN = IS_GEN | IS_ROWS | IS_COLS | IS_MIN | IS_MAX;
	const int GEN_SEED = GEN | SEED;
	const int BFS = IS_BFS | IS_VERT1;
	const int DIJKSTRA = IS_DIJKSTRA | IS_VERT1 | IS_VERT2;

	/* nie podano ziarna dla generatora */
	if ((prog_options ^ SEED) == 0)
		seed = time(NULL);

	if ((prog_options ^ HELP) == 0) {
		print_help();
		exit(EXIT_SUCCESS);

	} else if ((prog_options ^ GEN) == 0 || (prog_options ^ GEN_SEED) == 0) {
		if (rows < 1 || cols < 1) {
			PROGRAM_ERROR("rows and cols cannot be smaller than 1"); 
		} else if (min > max) {
			PROGRAM_ERROR("min cannot be greater than max");
		}

		Graph *graph = graph_generate_from_seed(rows, cols, min, max, seed);
		graph_print_to_stdout(graph);
		graph_free(graph);

	} else if ((prog_options ^ BFS) == 0) {
		Graph* graph = malloc(sizeof(*graph));
		int did_fail = graph_read_from_stdin(graph);

		if (did_fail) {
			graph_free(graph);
			PROGRAM_ERROR("invalid graph");
		} 
		if (vert1 < 0 || vert1 >= graph->cols * graph->rows) {
			graph_free(graph);
			PROGRAM_ERROR("invalid vertex index");
		}

		BFSResult *result = bfs(graph, vert1);
		bfs_print_result(result);
		bfs_result_free(result);
		graph_free(graph);

	} else if ((prog_options ^ DIJKSTRA) == 0) {
		Graph * graph = malloc(sizeof(*graph));
		int did_fail = graph_read_from_stdin(graph);

		if(did_fail) {
			graph_free(graph);
			PROGRAM_ERROR("invalid graph");
		}
		if(vert1 >= graph->cols * graph->rows || vert2 >= graph->cols * graph->rows) {
			graph_free(graph);
			PROGRAM_ERROR("dijkstra: the specified verticies are not the part of the graph");
		}

		DijkstraResult * result = dijkstra(graph, vert1);
		dijkstra_print_result(result);
		dijkstra_print_path(result, vert2);
		dijkstra_result_free(result);
		graph_free(graph);

	} else {
		PROGRAM_ERROR("unfamiliar combination of program options, for help run: graphalgo --help");
	}
}
