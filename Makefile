CFLAGS=-ggdb  -Wall -pedantic --std=c99 -Wmisleading-indentation
.DEFAULT_GOAL := all

objs/vertex_priority_queue.o: src/vertex_priority_queue.c src/vertex_priority_queue.h
	cc $(CFLAGS) -c src/vertex_priority_queue.c -o objs/vertex_priority_queue.o 

objs/graph.o: src/graph.c src/graph.h
	cc $(CFLAGS) -c src/graph.c -o objs/graph.o

objs/bfs.o: src/bfs.c src/bfs.h
	cc $(CFLAGS) -c src/bfs.c -o objs/bfs.o 

objs/dijkstra.o: src/dijkstra.h src/dijkstra.c
	cc $(CLAGS) -c src/dijkstra.c -o objs/dijkstra.o

build_test_pq: objs/vertex_priority_queue.o
	cc $(CFLAGS) tests/test_vertex_priority_queue.c objs/vertex_priority_queue.o -Isrc/ -o bin/test_pq 

graphalgo: src/main.c objs/graph.o objs/bfs.o
	cc $(CFLAGS) src/main.c objs/graph.o objs/bfs.o -o bin/graphalgo

build_test_dijkstra: objs/dijkstra.o objs/vertex_priority_queue.o objs/graph.o
	cc $(CFLAGS) tests/test_dijkstra.c objs/dijkstra.o objs/vertex_priority_queue.o objs/graph.o -Isrc/ -o bin/test_dijkstra 

build_test_bfs: objs/bfs.o objs/graph.o
	cc $(CFLAGS) tests/test_bfs.c objs/bfs.o objs/graph.o -Isrc/ -o bin/test_bfs

graphalgo: src/main.c objs/bfs.o objs/graph.o objs/dijkstra.o objs/vertex_priority_queue.o
	cc $(CFLAGS) src/main.c objs/bfs.o objs/graph.o objs/dijkstra.o objs/vertex_priority_queue.o -o bin/graphalgo

all:  graphalgo objs/vertex_priority_queue.o	

test: graphalgo build_test_pq build_test_dijkstra build_test_bfs
	@bash ./tests/main_args_tests.sh
	@bash ./tests/valgrind_tests.sh

clean:
	rm -rf objs/*
	rm -rf bin/*

