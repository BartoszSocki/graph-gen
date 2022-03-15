#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main() {
	Graph* g = graphGenerateFromSeed(10, 20, 0, 1, 1);
	graphPrintToStdout(g);
}
