#ifndef K_H
#define K_H

#include <stdio.h>
#include <stdlib.h>
#include "unweighted-directed-graph.h"

#define FORWARD 1
#define REVERSE 0

typedef struct __finish_map {
    size_t      v_id;
    size_t      finish_time;
} finish_map_t;

size_t* depth_first_search(graph_t* graph);
size_t dfs(vertice_t start_vertex, size_t* discovery_map, size_t* finish_time_map, size_t* finish_time, size_t* count, size_t parent_id, size_t* parent_map);
void print_finish_times(size_t* discovery_map, size_t len);
int compare_finish_times(const void* a, const void* b);
int compare_size_t(const void* a, const void* b);
size_t* find_strongly_connected_components(graph_t* reverse_graph, graph_t* forward_graph);
size_t* find_scc(graph_t* graph, finish_map_t* finish_map);

#endif