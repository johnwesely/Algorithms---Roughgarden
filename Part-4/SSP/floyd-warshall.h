#include <stdio.h>
#include <stdlib.h>

#ifndef FW_H
#define FW_H

int** floyd_warshall(graph_t* g);
void destroy_distances(int** distances, graph_t* g);
void print_shortest_distances(int** distances, graph_t* g);

#endif