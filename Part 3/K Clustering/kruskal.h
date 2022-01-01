#include <stdio.h>
#include <stdlib.h>
#include "weighted-undirected-graph.h"
#include "union-find.h"

#ifndef K_H
#define K_H

void sort_edges(graph_t* g);
size_t kruskal_mst(graph_t* g);
size_t k_clustering(graph_t* g, size_t k);

#endif