#include "kruskal.h"

int compare_edges(const void* a, const void* b) {
    edge_t* aa = (edge_t*)a;
    edge_t* bb = (edge_t*)b;

    if (aa->weight > bb->weight) {
        return 1;
    } else {
        return -1;
    }
}

void sort_edges(graph_t* g) {
    qsort(g->e_arr, g->e_count, sizeof(edge_t*), compare_edges);
}