#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define F 1
#define R 0

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

typedef struct __finish_map {
    size_t      v_id;
    size_t      finish_time;
} finish_map_t;

graph_t* create_graph(size_t v_count);
void append_edge(graph_t* g, size_t i, size_t j);
void destroy_graph(graph_t* g);
void destroy_edge_list(edge_t* e);
graph_t* read_graph(FILE* f, size_t direction);
size_t count_vertices(FILE* f);
size_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream);
void print_graph(graph_t* g);
size_t* depth_first_search(graph_t* g);
size_t dfs(vertice_t v, size_t* discovery_map, size_t* finish_time_map, size_t* finish_time, size_t* count);
void print_finish_times(size_t* dm, size_t len);
int compare_finish_times(const void* a, const void* b);
size_t* find_strongly_connected_components(FILE* f);
size_t* find_scc(FILE* f, finish_map_t* finish_map);

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

    size_t* scc = find_strongly_connected_components(f);

    size_t i = 0;
    printf("the size of the strongly connected components\n");
    while (scc[i]) {
        printf("%zu\n", scc[i++]);
    }

    free(scc);
    fclose(f);

    return EXIT_SUCCESS;
}
size_t* find_strongly_connected_components(FILE* f) {
    graph_t* g_rev = read_graph(f, R);
    size_t* finish_times = depth_first_search(g_rev);

    finish_map_t* finish_map = malloc(sizeof(finish_map_t) * g_rev->v_count);
    for (size_t i = 0; i < g_rev->v_count; ++i) {
        finish_map[i].v_id = i;
        finish_map[i].finish_time = finish_times[i];
    }
    qsort(finish_map, g_rev->v_count, sizeof(finish_map_t), compare_finish_times);

    for(size_t i = 0; i < g_rev->v_count; ++i) {
        printf("vertex %zu = finish time %zu\n", finish_map[i].v_id + 1, finish_map[i].finish_time);
    }

    destroy_graph(g_rev);
    free(finish_times);

    size_t* scc = find_scc(f, finish_map);
    free(finish_map);
    return scc;
}

size_t* find_scc(FILE* f, finish_map_t* finish_map) {
    graph_t* g = read_graph(f, F);
    size_t* scc = calloc(g->v_count, sizeof(size_t));
    size_t* discovery = calloc(g->v_count, sizeof(size_t));
    size_t* finish_times = calloc(g->v_count, sizeof(size_t));

    size_t j = 0; size_t count = 1; size_t finish_time = 1;
    for (size_t i = 0; i < g->v_count; ++i) {
        finish_map_t current = finish_map[i];
        if (discovery[current.v_id]) {
            continue;
        }
        count = 1;
        ++discovery[current.v_id];
        scc[j++] = dfs(g->vert_arr[current.v_id], discovery, finish_times, &finish_time, &count);
    }

    destroy_graph(g);
    free(discovery);
    free(finish_times);
    return scc;
}

size_t* depth_first_search(graph_t* g) {
    size_t* discovery = calloc(g->v_count, sizeof(size_t));
    size_t* finish_times = calloc(g->v_count, sizeof(size_t));
    size_t finish_time = 1;
    size_t count = 1;

    for(size_t i = 0; i < g->v_count; ++i) {
        if (discovery[i]) {
            continue;
        } 
        count = 1;
        ++discovery[i];
        dfs(g->vert_arr[i], discovery, finish_times, &finish_time, &count);
    }

    free(discovery);
    return finish_times;
}

// while nodes are left in vertice, see if node is discovered, if not,
// order node and call call dfs
size_t dfs(vertice_t v, size_t* discovery, size_t* finish_times, size_t* finish_time, size_t* count) {
    edge_t* current = v.head;

    while (current) {
        if (discovery[current->edge->id]) {
            current = current->next;
            continue;
        }

        ++discovery[current->edge->id];
        *count += 1;
        *count += dfs(*current->edge, discovery, finish_times, finish_time, count);
        current = current->next;
    }
    finish_times[v.id] = *finish_time;
    *finish_time += 1;
    return *count;
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

graph_t* read_graph(FILE* f, size_t direction) {
    fseek(f, 0, SEEK_SET);
    size_t vertice_c = count_vertices(f);
    graph_t* g = create_graph(vertice_c);

    size_t buff_size = 1024;
    char* buff = calloc(buff_size, 1);
    size_t i = 0; size_t j = 0;

    while (getline(&buff, &buff_size, f) != -1) {
        i = atoi(strtok(buff, " ")) - 1;
        j = atoi(strtok(0, " ")) - 1;
        (direction) ? append_edge(g, i, j) : append_edge(g, j, i);
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

void print_finish_times(size_t* dm, size_t len) {
    printf("finish times with depth first search\n");
    for (size_t i = 0; i < len; ++i) {
        printf("vertice %zu = %zu\n", i + 1, dm[i]);
    }
}

int compare_finish_times(const void* a, const void* b) {
    finish_map_t* aa = (finish_map_t*) a;
    finish_map_t* bb = (finish_map_t*) b;

    return (aa->finish_time < bb->finish_time);
}