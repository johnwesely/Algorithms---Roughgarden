#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "weighted-undirected-graph.h"

void pick_next(int* ret, int* weights, size_t* visited, size_t v_count);
int mst_weight(graph_t* g);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Prim takes one command line argument, a file representing a graph\n");
        return EXIT_FAILURE;
    }

    FILE* f = fopen(argv[1], "r");
    graph_t* g = read_graph(f);
    print_graph(g);
    
    printf("min weight of graph is %d\n", mst_weight(g));

    return EXIT_SUCCESS;
}

int mst_weight(graph_t* g) {
    size_t x_size = 0;
    size_t* visited = calloc(g->v_count, sizeof(size_t));
    int* weights = malloc(g->v_count * sizeof(int));
    int* next_ret = calloc(2, sizeof(int));    // pass to pick_next for next vertice and weight of edge pointing to vertice
    int weight = 0;

    for (size_t i = 0; i < g->v_count; ++i) {
        weights[i] = INT_MAX;
    }

    ++visited[0];
    ++x_size;
    edge_t* current = g->vert_arr[0].head;

    while (x_size < g->v_count) {
        while (current) {
            if (current->weight < weights[current->id]) {
                weights[current->id] = current->weight;
            }
            current = current->next;
        }
        pick_next(next_ret, weights, visited, g->v_count);
        current = g->vert_arr[next_ret[1]].head;
        weight += next_ret[0];
        printf("adding edge weight %d to sum\n", next_ret[0]);
        ++visited[next_ret[1]];
        ++x_size;
    }

    free(visited);
    free(next_ret);
    return weight;
}

void pick_next(int* ret, int* weights, size_t* visited, size_t v_count) {
    ret[0] = INT_MAX;   // weight
    ret[1] = 0;         // index

    for (size_t i = 0; i < v_count; ++i) {
        if (!visited[i] && weights[i] < ret[0]) {
            ret[1] = i;
            ret[0] = weights[i];
        }
    }
}
