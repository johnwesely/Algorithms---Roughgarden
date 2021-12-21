run: dij.o heap.o weighted-undirected-graph.o
	c99 -Wall -g dij.o heap.o weighted-undirected-graph.o -o run 

dij.o: dij.c
	c99 -c dij.c

heap.o: heap.c heap.h
	c99 -c heap.c

weighted-undirected-graph.o: weighted-undirected-graph.c weighted-undirected-graph.h
	c99 -c weighted-undirected-graph.c

clean:
	rm *.o run


