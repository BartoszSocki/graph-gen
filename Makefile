objs/priority_queue.o: src/priority_queue.c src/priority_queue.h
	cc -c src/priority_queue.c -o objs/priority_queue.o 

build_test_pq: objs/priority_queue.o
	cc tests/test_priority_queue.c objs/priority_queue.o -Isrc/ -o bin/test_pq 
test_pq: build_test_pq
	./bin/test_pq
clean:
	rm -rf objs/*
	rm -rf bin/*