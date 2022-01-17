#include "wdg.h"
#include "floyd-warshall.h"
#include <limits.h>

// helper function for floyd-warshall
// checks if current entry in memo table differences is greater than previous entry plus new weight
size_t distance_check(int a, int b, int c) {
    if (b == INT_MAX || c == INT_MAX) {
        return 0;
    }

    if (a > b + c) {
        return 1;
    }

    return 0;
}

// runs floyd-warshall algorithm to find all shortest paths in a graph
// returns malloced 2 dimensional array consisting of all shortest paths
int** floyd_warshall(graph_t* g) {
    int** distances = malloc(sizeof(int*) * g->v_count);

    for (size_t i = 0; i < g->v_count; ++i) {
        distances[i] = malloc(sizeof(int) * g->v_count);
        for (size_t j = 0; j < g->v_count; ++j) {
            distances[i][j] = INT_MAX;
        }
    }

    for (size_t i = 0; i < g->e_count; ++i) {
        if (distances[g->e_arr[i]->id_i][g->e_arr[i]->id_j] > g->e_arr[i]->weight) {
            distances[g->e_arr[i]->id_i][g->e_arr[i]->id_j] = g->e_arr[i]->weight;
            if (g->e_arr[i]->id_i == g->e_arr[i]->id_j && g->e_arr[i]->weight < 0) {
                printf("negative cycle detetcted\n");
                destroy_distances(distances, g);
                return 0;
            }
        }
    }

    for (size_t i = 0; i < g->v_count; ++i) {
        distances[i][i] = 0;
    }

    for (size_t k = 0; k < g->v_count; ++k) {
        for (size_t i = 0; i < g->v_count; ++i) {
            for (size_t j = 0; j < g->v_count; ++j) {
                if (distance_check(distances[i][j], distances[i][k], distances[k][j])) {
                    distances[i][j] = distances[i][k] + distances[k][j];
                    if (i == j) {
                        printf("negative cycle detected\n");
                        destroy_distances(distances, g);
                        return 0;
                    }
                }
            }
        }
    }

    return distances;
}

void destroy_distances(int** distances, graph_t* g) {
    for (size_t i = 0; i < g->v_count; ++i) {
        free(distances[i]);
    }
    free(distances);
}

void print_shortest_distances(int** distances, graph_t* g) {
    for (size_t i = 0; i < g->v_count; ++i) {
        for (size_t j = 0; j < g->v_count; ++j) {
            printf("Vertex %zu to %zu = %d\n", i, j, distances[i][j]);
        }
    }
}