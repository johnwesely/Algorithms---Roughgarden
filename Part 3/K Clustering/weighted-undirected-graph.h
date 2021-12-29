#include <stdio.h>
#include <stdlib.h>
#include "union-find.h"

#ifndef WUD_H
#define WUD_H

typedef struct __edge  edge_t;
typedef struct __node  node_t;
typedef struct __graph graph_t;

// edge of weighted undirected graph
struct __edge {
    node_t* i;
    size_t  id_i;
    node_t* j;
    size_t  id_j;
    size_t  weight;
};

// node of weighted undirected graph
struct __node {
    size_t    id;
    size_t* parent;
    edge_t*   head;
}

// graph
struct __graph {
    size_t        v_count;
    size_t        e_count;
    node_t*       v_arr;
    node_t*       e_arr;
    union_find_t* uf;
}

// Weighted Undirected Graph API
graph_t* read_graph(FILE* f);     // read graph from file
void print_graph(graph_t* g);     // print graph to terminal
void destroy_graph(graph_t* g);   // free graph

#endif 
