#include "kruskal.h"

int compare_edges(const void* a, const void* b) {
    edge_t* edge_a = *(edge_t**)a;
    edge_t* edge_b = *(edge_t**)b;

    if (edge_a->weight > edge_b->weight) {
        return 1;
    } else {
        return -1;
    }
}

void sort_edges(graph_t* g) {
    qsort(g->e_arr, g->e_count, sizeof(edge_t*), compare_edges);
}

size_t kruskal_mst(graph_t* g) {          // run kruskals to find size of MST
    return k_clustering(g, 1);
}

size_t k_clustering(graph_t* g, size_t k) {
    sort_edges(g);
    size_t size_x = g->v_count;
    size_t edge_index = 0;
    size_t mst_sum = 0;
    size_t max_cluster = 0;
    size_t i; size_t j;

    while (size_x > k) {                  // run Kruskal until k clusters remain
        i = g->e_arr[edge_index]->id_i;   // variables for legibility 
        j = g->e_arr[edge_index]->id_j;
        if (find(i, g->uf) != find(j, g->uf)) {
            mst_sum += g->e_arr[edge_index]->weight;
            make_union(i, j, g->uf);
            --size_x;
        }
        ++edge_index;
    }

    // find the next edge that would be part of MST
    while (size_x > 1) {
        i = g->e_arr[edge_index]->id_i;
        j = g->e_arr[edge_index]->id_j;
        if (find(i, g->uf) != find(j, g->uf)) {
            max_cluster = g->e_arr[edge_index]->weight;
            break;
        }
        ++edge_index;
    }

    if (k == 1) {         // if k is one, return weight of MST return mst_sum;
        return mst_sum;
    } else {
        return max_cluster;
    }
}