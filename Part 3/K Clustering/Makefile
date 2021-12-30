run: main.o union-find.o weighted-undirected-graph.o
	c99 -Wall -g main.o union-find.o weighted-undirected-graph.o -o run

main.o: main.c
	c99 -c -g main.c

union-find.o: union-find.c union-find.h
	c99 -c -g union-find.c

weighted-undirected-graph.o: weighted-undirected-graph.c weighted-undirected-graph.h
	c99 -c -g weighted-undirected-graph.c

clean:
	rm *.o run
