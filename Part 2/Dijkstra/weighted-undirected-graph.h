/* format of graph file is as follows:
**     1 2,1 4,6
**     2 1,1 4,1 3,1 5,4
**     3 2,1 5,1
**     4 1,1 2,7 5,9
**     5 2,4 4,9 3,1
** where the first value in every line is the node id
** the subsequent comma seperated values are the nodes pointed to by that lines node
** and the weight of the edge between the nodes
*/

#include <stdio.h>
#include <stdlib.h>

#ifndef WUD_H
#define WUD_H

typedef struct __edge edge_t;
typedef struct __vertice vertice_t;
typedef struct __graph graph_t;

// edge of a weighted undirected graph
struct __edge {
    size_t      id;
    vertice_t*  edge;
    size_t      weight;
    edge_t*     next;
};

// vertice of graph
struct __vertice {
    size_t      id;
    edge_t*     head;
};

// weighted undirected graph
struct __graph {
    size_t      v_count;
    vertice_t*  vert_arr;
};

// Weighted Undirected Graph API
graph_t* read_graph(FILE* f);     // read graph from file
void print_graph(graph_t* g);     // print graph to terminal
void destroy_graph(graph_t* g);   // free graph

#endif