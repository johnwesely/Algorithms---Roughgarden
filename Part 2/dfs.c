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
size_t* depth_first_search(graph_t* g);
void dfs(vertice_t v, size_t* discovery_map, size_t* order);
void print_discovery_map(size_t* dm, size_t len);

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
    size_t* discovery_map = depth_first_search(g);
    printf("number of vertices in g = %zu\n", g->v_count);
    print_discovery_map(discovery_map, g->v_count);
    destroy_graph(g);

    return EXIT_SUCCESS;
}

size_t* depth_first_search(graph_t* g) {
    size_t* discovery_map = calloc(g->v_count, sizeof(size_t));
    size_t order = 1;

    for(size_t i = 0; i < g->v_count; ++i) {
        if (discovery_map[i]) {
            continue;
        } 

        discovery_map[i] = order++;
        dfs(g->vert_arr[i], discovery_map, &order);
    }

    return discovery_map;
}

// while nodes are left in vertice, see if node is discovered, if not,
// order node and call call dfs
void dfs(vertice_t v, size_t* discovery_map, size_t* order) {
    edge_t* current = v.head;

    while (current) {
        if (discovery_map[current->edge->id]) {
            current = current->next;
            continue;
        }
        discovery_map[current->edge->id] = *order;
        *order += 1;
        dfs(*current->edge, discovery_map, order);
        current = current->next;
    }
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
    if (e) {
        if (e->next) destroy_edge_list(e->next);
        free(e);
    } 
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
    char* vert_i;
    char* vert_j;

    if (!buff) {
        perror("allocation for count vertices buffer failed ");
        return 0;
    }

    size_t v_count = 0;

    while (getline(&buff, &buff_size, f) != -1) {
        vert_i = strtok(buff, " ");
        vert_j = strtok(0, " ");
        if (atoi(vert_i) > v_count) v_count = atoi(vert_i);
        if (atoi(vert_j) > v_count) v_count = atoi(vert_j);
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

void print_discovery_map(size_t* dm, size_t len) {
    printf("order of discovery with depth first search\n");
    for (size_t i = 0; i < len; ++i) {
        printf("vertice %zu = %zu\n", i + 1, dm[i]);
    }
}