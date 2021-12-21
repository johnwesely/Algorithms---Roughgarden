#include <stdio.h>
#include <stdlib.h>

#ifndef WUD_H
#define WUD_H

typedef struct __edge edge_t;
typedef struct __vertice vertice_t;
typedef struct __graph graph_t;

struct __edge {
    size_t      id;
    vertice_t*  edge;
    size_t      weight;
    edge_t*     next;
};

struct __vertice {
    size_t      id;
    edge_t*     head;
};

struct __graph {
    size_t      v_count;
    vertice_t*  vert_arr;
};

// Weighted Undirected Graph API
graph_t* read_graph(FILE* f);
void print_graph(graph_t* g);
void destroy_graph(graph_t* g);

#endif