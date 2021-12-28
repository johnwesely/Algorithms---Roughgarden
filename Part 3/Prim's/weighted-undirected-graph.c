#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "weighted-undirected-graph.h"

graph_t* create_graph(size_t v_count);
void append_edge(graph_t* g, size_t i, size_t j, int w);
void destroy_graph(graph_t* g);
void destroy_edge_list(edge_t* e);
graph_t* read_graph(FILE* f);
size_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream);
void print_graph(graph_t* g);

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
void append_edge(graph_t* g, size_t i, size_t j, int w) {
    edge_t* edge = malloc(sizeof(edge_t));
    edge->id = j;
    edge->next = g->vert_arr[i].head;
    edge->edge = &g->vert_arr[j];
    edge->weight = w;
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

// reads graph from file "f"
graph_t* read_graph(FILE* f) {
    fseek(f, 0, SEEK_SET);

    size_t buff_size = 1024;
    char* buff = calloc(buff_size, 1);

    if (getline(&buff, &buff_size, f) == -1) {
        printf("empty_file\n");
        exit(0);
    }

    graph_t* g = create_graph(atoi(strtok(buff, " ")));

    size_t i = 0; size_t j = 0; int w = 0;

    while (getline(&buff, &buff_size, f) != -1) {   
        i = atoi(strtok(buff, " ,\t\r\f\v\n")) - 1;
        j = atoi(strtok(0, " ")) - 1;
        w = atoi(strtok(0, " ,\t\r\f\v"));
        append_edge(g, i, j, w);
        append_edge(g, j, i, w);
    }

    free(buff); 

    return g;
}

// prints graph for debugging purposes
void print_graph(graph_t* g) {
    edge_t* edge;
    for (size_t i = 0; i < g->v_count; ++i) {
        printf("Vertice %zu -> ", g->vert_arr[i].id + 1);
        edge = g->vert_arr[i].head;

        while (edge) {
            printf("%zu, %d -> ", edge->id + 1, edge->weight);
            edge = edge->next;
        }

        printf("NULL\n");
    }
}