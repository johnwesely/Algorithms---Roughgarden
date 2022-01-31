#include "unweighted-directed-graph.h"

// allocates graph in memory with "v_count" vertices
graph_t* create_graph(size_t v_count) {
    graph_t* ret = malloc(sizeof(graph_t));
    ret->vert_arr = calloc(v_count, sizeof(vertice_t));

    for (size_t i = 0; i < v_count; ++i) {
        ret->vert_arr[i].id = i;
    }

    ret->v_count = v_count;
    return ret;
}

// appends edge ("i", "j") to vertice "i" in graph "g"
void append_edge(graph_t* g, size_t i, size_t j) {
    edge_t* edge = malloc(sizeof(edge_t));
    edge->id = j;
    edge->next = g->vert_arr[i].head;
    edge->edge = &g->vert_arr[j];
    g->vert_arr[i].head = edge;
}

// frees graph 
void destroy_graph(graph_t* g) {
    for (size_t i = 0; i < g->v_count; ++i) {
        destroy_edge_list(g->vert_arr[i].head);
    }

    free(g->vert_arr);
    free(g);
}

// frees vertice in graph
void destroy_edge_list(edge_t* e) {
    if (e) {
        if (e->next) destroy_edge_list(e->next);
        free(e);
    } 
}

// prints graph for debugging purposes
void print_graph(graph_t* g) {
    edge_t* edge;
    for (size_t i = 0; i < g->v_count; ++i) {
        printf("Vertice %zu -> ", g->vert_arr[i].id + 1);
        edge = g->vert_arr[i].head;

        while (edge) {
            printf("%zu -> ", edge->id + 1);
            edge = edge->next;
        }

        printf("NULL\n");
    }
}