#include "heap.h"
#include "weighted-undirected-graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

size_t* dijkstra_shortest_distance(size_t s, graph_t* g);
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
    print_graph(g);

    size_t* distances = dijkstra_shortest_distance(0, g);
    for (size_t i = 0; i < g->v_count; ++i) {
        printf("%zu  %zu\n", i + 1, distances[i]);
    }

    size_t print_arr[] = { 7, 37, 59, 82, 99, 115, 133, 165, 188, 197 };

    printf("chosen vertices are\n");
    for (size_t i = 0; i < 10; ++i) {
        printf("%zu,", distances[print_arr[i] - 1]);
    }

    free(distances);
    destroy_graph(g);
    fclose(f);

    return EXIT_SUCCESS;
}


size_t* dijkstra_shortest_distance(size_t s, graph_t* g) {
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
        current = current->next;
    }

    size_t next_vertice = 0;
    while ((next_vertice = pop_heap(pq)) != UINT_MAX) {
        dij(next_vertice, g, distances, visited);
    }


    free(visited);
    destroy_heap(pq);

    return distances;
}

void dij(size_t v, graph_t* g, size_t* distances, size_t* visited) {
    edge_t* current = g->vert_arr[v].head;
    heap_t* pq = create_heap();
    printf("adding vertex %zu to X\n", v + 1);
    if (v + 1 == 97) {
        printf("lastbeforeseg\n");
    }
    ++visited[v];

    while (current) {
        if (distances[v] + current->weight < distances[current->id]) {
            distances[current->id] = distances[v] + current->weight;
            if (current->id == 80) {
                printf("!!!!shortest path to %zu is %zu\n", current->id + 1, distances[current->id]);
            }
            dij_pair_t dp = { current->id, distances[current->id]};
            push_heap(dp, pq);
        }
        current = current->next;
    }
    size_t next_vertice = 0;
    while((next_vertice = pop_heap(pq)) != UINT_MAX) {
        printf("next vertice is %zu\n", next_vertice);
        if (!visited[next_vertice]) {
            dij(next_vertice, g, distances, visited);
        }
    }

    destroy_heap(pq);
}
