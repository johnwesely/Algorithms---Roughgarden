#include <stdio.h>
#include <stdlib.h>
#include "union-find.h"

#ifndef WUD_H
#define WUD_H

typedef struct __edge  edge_t;
typedef struct __node  node_t;
typedef struct __graph graph_t;
typedef struct __edge_list edge_list_t;

// edge of weighted undirected graph
struct __edge {
    node_t* i;
    size_t  id_i;
    node_t* j;
    size_t  id_j;
    size_t  weight;
};

// list of edges pointed to by every node
struct __edge_list {
    size_t       id;
    edge_t*      edge;
    edge_list_t* next;
};

// node of weighted undirected graph
struct __node {
    size_t         id;
    edge_list_t*   head;
};

// graph
struct __graph {
    size_t        v_count;
    size_t        e_count;
    node_t*       v_arr;
    edge_t**      e_arr;
    union_find_t* uf;
};

// Weighted Undirected Graph API
graph_t* read_graph(FILE* f);                                             // read graph from file
graph_t* create_graph(size_t v_count, size_t e_count);                    // create graph
void print_graph(graph_t* g);                                             // print graph to terminal
void destroy_graph(graph_t* g);                                           // free graph
void append_edge(graph_t* g, size_t i, size_t j, size_t w, size_t index); // creates and appends edge to graph

#endif 
