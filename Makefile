CFLAGS=-ggdb -Wall -pedantic --std=c99
.DEFAULT_GOAL := all

objs/vertex_priority_queue.o: src/vertex_priority_queue.c src/vertex_priority_queue.h
	cc $(CFLAGS) -c src/vertex_priority_queue.c -o objs/vertex_priority_queue.o 

objs/graph.o: src/graph.c src/graph.h
	cc $(CFLAGS) -c src/graph.c -o objs/graph.o

objs/bfs.o: src/bfs.c src/bfs.h
	cc $(CFLAGS) -c src/bfs.c -o objs/bfs.o 

build_test_pq: objs/vertex_priority_queue.o
	cc $(CFLAGS) tests/test_vertex_priority_queue.c objs/vertex_priority_queue.o -Isrc/ -o bin/test_pq 

graphalgo: src/main.c objs/graph.o src/bfs.o
	cc $(CFLAGS) src/main.c objs/graph.o src/bfs.o -o bin/graphalgo

all:  graphalgo objs/vertex_priority_queue.o	

test: 
	@# PriorityQueue tests
	@./bin/test_pq 1>/dev/null && echo "Vertex Priority Queue: passed" || echo "Vertex Priorty Queue: failed" 
	@# Invalid Input tests
	@bash ./tests/main_args_tests.sh

clean:
	rm -rf objs/*
	rm -rf bin/*

