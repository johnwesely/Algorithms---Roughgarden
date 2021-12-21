#include "heap.h"
#include "weighted-undirected-graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

size_t dijkstra_shortest_distance(size_t s, size_t t, graph_t* g);
void dij(size_t v, graph_t* g, size_t* distances, size_t* visited);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("include a file dingus\n");
        return EXIT_FAILURE;
    }

    FILE* f = fopen(argv[1], "r");

    if (!f) {
        perror("failure to load file ");
        return EXIT_FAILURE;
    }

    graph_t* g = read_graph(f);
    size_t shortest_distance = dijkstra_shortest_distance(1, 2, g);
    printf("shortest distance is %zu\n", shortest_distance);

    destroy_graph(g);

    return EXIT_SUCCESS;
}

size_t dijkstra_shortest_distance(size_t s, size_t t, graph_t* g) {
    size_t* distances = malloc(g->v_count * sizeof(size_t));
    size_t* visited = calloc(g->v_count, sizeof(size_t));

    for (size_t i = 0; i < g->v_count; ++i) {
        distances[i] = UINT_MAX;
    }

    distances[s] = 0;
    ++visited[s];

    heap_t* pq = create_heap();

    edge_t* current = g->vert_arr[s].head;

    while (current) {
        distances[current->id] = current->weight;
        dij_pair_t dp = { current->id, current->weight };
        push_heap(dp, pq);
    }

    size_t next_vertice = 0;
    while ((next_vertice = pop_heap(pq)) != UINT_MAX) {
        dij(next_vertice, g, distances, visited);
    }

    size_t ret = distances[t];
    free(distances);
    free(visited);
    destroy_heap(pq);

    return distances[t];
}

void dij(size_t v, graph_t* g, size_t* distances, size_t* visited) {
    edge_t* current = g->vert_arr[v].head;
    heap_t* pq = create_heap();
    ++visited[v];

    while (current) {
        if (distances[v] + current->weight < distances[current->id]) {
            distances[current->id] = distances[v] + current->weight;
            dij_pair_t dp = { current->id, distances[current->id]};
            push_heap(dp, pq);
        }
    }

    size_t next_vertice = 0;
    while((next_vertice = pop_heap(pq)) != UINT_MAX) {
        if (!visited[next_vertice]) {
            dij(next_vertice, g, distances, visited);
        }
    }

    destroy_heap(pq);
}
