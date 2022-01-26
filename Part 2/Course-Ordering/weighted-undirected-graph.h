#ifndef WUG_H
#define WUG_H

#include <stdio.h>
#include <stdlib.h>
#include "linked-list.h"

// vertex of graph
typedef struct __vertex {
    size_t         id;
    linked_list_t  edges;
    linked_list_t  pre_reqs;
} vertex_t;

// unweighted directed graph
typedef struct __directed_graph {
    size_t    vertex_count;
    vertex_t* vertices;
} graph_t;

void destroy_graph(graph_t* graph);
void append_edge(graph_t* graph, size_t v_id, size_t j_id);

#endif