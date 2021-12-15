#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct __edge edge_t;
typedef struct __vertice vertice_t;
typedef struct __graph graph_t;

struct __edge {
    size_t      id;
    vertice_t*  edge;
    edge_t*  next;
};

struct __vertice {
    size_t      id;
    edge_t*     head;
};

struct __graph {
    size_t      v_count;
    vertice_t*  vert_arr;
};

graph_t* create_graph(size_t v_count);
void append_edge(graph_t* g, size_t i, size_t j);
void destroy_graph(graph_t* g);
void destroy_edge_list(edge_t* e);
graph_t* read_graph(FILE* f);
size_t count_vertices(FILE* f);
size_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream);
void print_graph(graph_t* g);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("dfs takes one command line argument, a file containing a text representation of a directed graph\n");
        return EXIT_FAILURE;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("unable to open file: ");
        return EXIT_FAILURE;
    }

    graph_t* g = read_graph(f);

    fclose(f);
    
    print_graph(g);
    destroy_graph(g);

    return EXIT_SUCCESS;
}

graph_t* create_graph(size_t v_count) {
    graph_t* ret = malloc(sizeof(graph_t));
    ret->vert_arr = calloc(v_count, sizeof(vertice_t));

    for (size_t i = 0; i < v_count; ++i) {
        ret->vert_arr[i].id = i;
    }

    ret->v_count = v_count;
    return ret;
}

void append_edge(graph_t* g, size_t i, size_t j) {
    edge_t* edge = malloc(sizeof(edge_t));
    edge->id = j;
    edge->next = g->vert_arr[i].head;
    edge->edge = &g->vert_arr[j];
    g->vert_arr[i].head = edge;
}

void destroy_graph(graph_t* g) {
    for (size_t i = 0; i < g->v_count; ++i) {
        destroy_edge_list(g->vert_arr[i].head);
    }

    free(g->vert_arr);
    free(g);
}

void destroy_edge_list(edge_t* e) {
    if (e->next) destroy_edge_list(e->next);
    free(e);
}

graph_t* read_graph(FILE* f) {
    size_t vertice_c = count_vertices(f);
    graph_t* g = create_graph(vertice_c);

    size_t buff_size = 1024;
    char* buff = calloc(buff_size, 1);
    size_t i = 0; size_t j = 0;

    while (getline(&buff, &buff_size, f) != -1) {
        i = atoi(strtok(buff, " ")) - 1;
        j = atoi(strtok(0, " ")) - 1;
        append_edge(g, i, j);
    }

    free(buff);
    return g;
}

size_t count_vertices(FILE* f) {
    size_t buff_size = 256;
    char* buff = calloc(buff_size, 1);
    char* vert;

    if (!buff) {
        perror("allocation for count vertices buffer failed ");
        return 0;
    }

    size_t v_count = 0;

    while (getline(&buff, &buff_size, f) != -1) {
        vert = strtok(buff, " ");
        if (atoi(vert) > v_count) ++v_count;
    }

    free(buff);
    fseek(f, 0, SEEK_SET);
    return v_count;
}

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
