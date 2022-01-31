#ifndef UDG_H
#define UDG_H

#include <stdlib.h>
#include <stdio.h>

typedef struct __edge edge_t;
typedef struct __vertice vertice_t;
typedef struct __graph graph_t;

struct __edge {
    size_t      id;
    vertice_t*  edge;
    edge_t*  next;
};

struct __vertice {
    size_t      id;
    edge_t*     head;
};

struct __graph {
    size_t      v_count;
    vertice_t*  vert_arr;
};

graph_t* create_graph(size_t v_count);
void append_edge(graph_t* g, size_t i, size_t j);
void destroy_graph(graph_t* g);
void destroy_edge_list(edge_t* e);
void print_graph(graph_t* g);

#endif