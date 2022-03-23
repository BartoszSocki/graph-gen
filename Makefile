CFLAGS=-ggdb -Wall -pedantic --std=c99
.DEFAULT_GOAL := all

objs/vertex_priority_queue.o: src/vertex_priority_queue.c src/vertex_priority_queue.h
	cc $(CFLAGS) -c src/vertex_priority_queue.c -o objs/vertex_priority_queue.o 

objs/graph.o: src/graph.c src/graph.h
	cc $(CFLAGS) -c src/graph.c -o objs/graph.o

objs/dijkstra.o: src/dijkstra.h src/dijkstra.c
	cc $(CLAGS) -c src/dijkstra.c -o objs/dijkstra.o

build_test_pq: objs/vertex_priority_queue.o
	cc $(CFLAGS) tests/test_vertex_priority_queue.c objs/vertex_priority_queue.o -Isrc/ -o bin/test_pq 

build_test_dijkstra: objs/dijkstra.o objs/vertex_priority_queue.o objs/graph.o
	cc $(CFLAGS) tests/test_dijkstra.c objs/dijkstra.o objs/vertex_priority_queue.o objs/graph.o -Isrc/ -o bin/test_dijkstra 

graphalgo: src/main.c objs/graph.o objs/dijkstra.o objs/vertex_priority_queue.o
	cc $(CFLAGS) src/main.c objs/graph.o objs/dijkstra.o objs/vertex_priority_queue.o -o bin/graphalgo


graphalgo_options_test_1:
	@-./bin/graphalgo -g 2> tests/out1
	@echo "graphalgo: not enough options passed" > tests/out2
	@diff tests/out1 tests/out2
	@rm tests/out1 tests/out2

graphalgo_options_test_2:
	@-./bin/graphalgo -g -g 2> tests/out1
	@echo "graphalgo: -g, --generate passed multiple times" > tests/out2
	@diff tests/out1 tests/out2
	@rm tests/out1 tests/out2

graphalgo_options_test_3:
	@-./bin/graphalgo -g -r4 -c4 -n0 -x1 -s0 > tests/out1
	@-./bin/graphalgo --generate --rows=4 --cols=4 --min=0 --max=1 --seed=0 > tests/out2
	@diff tests/out1 tests/out2
	@rm tests/out1 tests/out2

graphalgo_options_test_4:
	./bin/graphalgo -g -r 10 -c 10 --min=0 --max=1 -s 0

all:  graphalgo objs/vertex_priority_queue.o	

test: graphalgo build_test_pq build_test_dijkstra 
	@./bin/test_dijkstra 1>/dev/null && echo "Dijkstra : passed" || echo "Dijkstra: failed" 
	@# PriorityQueue tests
	@./bin/test_pq 1>/dev/null && echo "Vertex Priority Queue: passed" || echo "Vertex Priorty Queue: failed" 
	@# Invalid Input tests
	-make graphalgo_options_test_1 2> /dev/null
	-make graphalgo_options_test_2 2> /dev/null
	-make graphalgo_options_test_3 2> /dev/null

clean:
	rm -rf objs/*
	rm -rf bin/*

