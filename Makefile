CFLAGS=-ggdb -Wall -pedantic --std=c99
.DEFAULT_GOAL := all
objs/vertex_priority_queue.o: src/vertex_priority_queue.c src/vertex_priority_queue.h
	cc $(CFLAGS) -c src/vertex_priority_queue.c -o objs/vertex_priority_queue.o 

build_test_pq: objs/vertex_priority_queue.o
	cc $(CFLAGS) tests/test_vertex_priority_queue.c objs/vertex_priority_queue.o -Isrc/ -o bin/test_pq 

objs/graph.o: src/graph.c src/graph.h
	cc $(CFLAGS) -c src/graph.c -o objs/graph.o

graphalgo: src/main.c objs/graph.o
	cc $(CFLAGS) src/main.c objs/graph.o -o bin/graphalgo

test_pq: build_test_pq
	./bin/test_pq

clean:
	rm -rf objs/*
	rm -rf bin/*

test_all:
	-make graphalgo_options_test_1 2> /dev/null
	-make graphalgo_options_test_2 2> /dev/null
	-make graphalgo_options_test_3 2> /dev/null

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
test: graphalgo_options_test_4 test_pq
