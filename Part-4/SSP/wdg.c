#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "wdg.h"

// allocates graph and returns pointer
graph_t* create_graph(size_t v_count, size_t e_count) {
    graph_t* ret = malloc(sizeof(graph_t));
    ret->v_count = v_count;
    ret->e_count = e_count;
    ret->v_arr = calloc(v_count, sizeof(node_t));

    for (size_t i = 0; i < v_count; ++i) {
        ret->v_arr[i].id = i;
    }

    ret->e_arr = malloc(sizeof(edge_t*) * e_count);

    return ret;
}

// destroys edge list
void destroy_edge_list(edge_list_t* el) {
    if (!el) return;

    edge_list_t* current = el;
    if (current->next) {
        destroy_edge_list(current->next);
    }
    free(current);
}

// destroys graph
void destroy_graph(graph_t* g) {
    for (size_t i = 0; i < g->v_count; ++i) {
        if (g->v_arr[i].head) {
            destroy_edge_list(g->v_arr[i].head);
        }
    }

    for (size_t i = 0; i < g->e_count; ++i) {
        free(g->e_arr[i]);
    }

    free(g->v_arr);
    free(g->e_arr);
    free(g);
}

// appends to edgelist
void append_edge_list(edge_t* edge, size_t id, node_t* node) {
    edge_list_t* el = malloc(sizeof(edge_list_t)); 
    el->id = id;
    el->edge = edge;
    el->next = node->head;
    node->head = el;
}

// creates edge, adds edge to e_arr, and appends list entries to nodes
void append_edge(graph_t* g, size_t i, size_t j, int w, size_t index) {
    edge_t* edge = malloc(sizeof(edge_t));
    g->e_arr[index] = edge;
    edge->id_i = i;
    edge->id_j = j;
    edge->weight = w;
    edge->i = &g->v_arr[i];
    edge->j = &g->v_arr[j];
    append_edge_list(edge, j, &g->v_arr[i]);
}

void print_graph(graph_t* g) {
    edge_list_t* el;
    for (size_t i = 0; i < g->v_count; ++i) {
        printf("Node %zu -> ", g->v_arr[i].id + 1);
        el = g->v_arr[i].head;

        while (el) {
            printf("%zu, %d -> ", el->id + 1, el->edge->weight);
            el = el->next;
        }

        printf("NULL\n");
    }
}

void print_edges(graph_t* g) {
    for (size_t i = 0; i < g->e_count; ++i) {
        printf("i = %zu, j = %zu, w = %d\n", g->e_arr[i]->id_i + 1, g->e_arr[i]->id_j + 1, g->e_arr[i]->weight);
    }
}