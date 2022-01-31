#include "Korasaju.h"

// High level function for finding SCC's, returns array with parent id mapped to index of child id
size_t* find_strongly_connected_components(graph_t* reverse_graph, graph_t* forward_graph) {
    size_t count = reverse_graph->v_count;
    size_t* finish_times = depth_first_search(reverse_graph);        // calculate finishing order
    destroy_graph(reverse_graph);

    // map finishing times to respective vertice id's and reverse sort by finishing time
    finish_map_t* finish_map = malloc(sizeof(finish_map_t) * count);
    for (size_t i = 0; i < count; ++i) {
        finish_map[i].v_id = i;
        finish_map[i].finish_time = finish_times[i];
    }
    qsort(finish_map, count, sizeof(finish_map_t), compare_finish_times);
    
    free(finish_times);

    size_t* scc_parent_map = find_scc(forward_graph, finish_map);

    free(finish_map);

    return scc_parent_map;
}

// second DFS with vertices searched in reverse order of finishing times
size_t* find_scc(graph_t* g, finish_map_t* finish_map) {
    size_t* scc_sizes = calloc(g->v_count, sizeof(size_t));     // array of SCC sizes
    size_t* discovery = calloc(g->v_count, sizeof(size_t));     // boolean array to check if vertice has been visited
    size_t* finish_times = calloc(g->v_count, sizeof(size_t));  // array of finish times, not used in this pass
    size_t* scc_parent_map = calloc(g->v_count, sizeof(size_t));    // map of vertice ids to parent ids

    size_t j = 0; size_t count = 1; size_t finish_time = 1;
    for (size_t i = 0; i < g->v_count; ++i) {                   // iterate through vertices sorted by finish time in descending order
        finish_map_t current = finish_map[i];
        if (discovery[current.v_id]) {
            continue;
        }
        count = 0;
        ++discovery[current.v_id];                 // add current vertex id to set of discovered vertices
        scc_parent_map[current.v_id] = current.v_id;   // set parent of current vertex to current vertex (leader of SCC)
        // call dfs on leader of SCC
        scc_sizes[j++] = dfs(g->vert_arr[current.v_id], discovery, finish_times, &finish_time, &count, current.v_id, scc_parent_map);
    }

    destroy_graph(g);
    free(discovery);
    free(finish_times);
    free(scc_sizes);
    return scc_parent_map;
}

// top level function for depth first search of entire graph
size_t* depth_first_search(graph_t* g) {
    size_t* discovery = calloc(g->v_count, sizeof(size_t));      // boolean array to check if vertice has been visited
    size_t* finish_times = calloc(g->v_count, sizeof(size_t));   // array of finish times
    size_t* parent_map = calloc(g->v_count, sizeof(size_t));     // array of parent ids
    size_t finish_time = 1;                                      // finish time counter
    size_t count = 1;                                            // count, not used in this pass

    for(size_t i = 0; i < g->v_count; ++i) {
        if (discovery[i]) {                    // if vertice has been searched continue
            continue;
        } 
        count = 0;
        ++discovery[i];                        // set vertice's place in discovery to true
        dfs(g->vert_arr[i], discovery, finish_times, &finish_time, &count, i, parent_map);
    }

    free(discovery);
    free(parent_map);
    return finish_times;
}

// recursively searches graph until all points reachable from "v" are found
// returns number of vertices reachable from "v"
size_t dfs(vertice_t v, size_t* discovery, size_t* finish_times, size_t* finish_time, size_t* count, size_t parent_id, size_t* parent_map) {
    edge_t* current_vertex = v.head;

    while (current_vertex) {                         // iterate through all vertices pointed to by "v"
        if (discovery[current_vertex->edge->id]) {
            current_vertex = current_vertex->next;          // if vertice has been seen, continue
            continue;
        }

        parent_map[current_vertex->edge->id] = parent_id;  // set vertexes parent to parent_id
        ++discovery[current_vertex->edge->id];             // mark vertex as found
        // call dfs on each vertice pointed to by "v"
        *count = dfs(*current_vertex->edge, discovery, finish_times, finish_time, count, parent_id, parent_map);
        current_vertex = current_vertex->next;      // cycle to next vertice
    }
    if (finish_times[v.id] == 0) {                  
        finish_times[v.id] = *finish_time;          // update finish time
        *finish_time += 1;
    }

    return *count + 1;                              // base case adds one to count
}

// prints finish times for debugging purposes
void print_finish_times(size_t* dm, size_t len) {
    printf("finish times with depth first search\n");
    for (size_t i = 0; i < len; ++i) {
        printf("vertice %zu = %zu\n", i + 1, dm[i]);
    }
}

// compar functions for qsort
int compare_finish_times(const void* a, const void* b) {
    finish_map_t* aa = (finish_map_t*) a;
    finish_map_t* bb = (finish_map_t*) b;

    return (aa->finish_time < bb->finish_time);
}

int comp_size_t(const void* a, const void* b) {
    return (*(size_t*) a < *(size_t*) b);
}