/* implementation of random contraction algorithm for an undirected graph
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

typedef struct __edge_list edge_list;
typedef struct __edge edge_t; 

// a vertex is a linked list of edges
struct __edge_list {
    edge_t* edge;
    edge_list* next;
}; 

// an edge points to vertices
struct __edge {
    edge_list* i;
    edge_list* j;
};

typedef struct __graph {
    edge_list** vertice_arr;
    edge_t**    edge_arr;
    size_t      edge_count;
    size_t      vertice_count;
} graph_t;

void append_edge(edge_t* e);
void a_e(edge_list* v, edge_t* e);
void collapse_edge(edge_t* e);
size_t random_graph_contraction(graph_t* g);
size_t random_graph_contraction_trials(size_t n, graph_t* g);

int main(int argc, char* argv[]) {
    srand(time(0));
    edge_list** v_arr = calloc(4, sizeof(edge_list*));
    v_arr[0] = calloc(1, sizeof(edge_list));
    v_arr[1] = calloc(1, sizeof(edge_list));
    v_arr[2] = calloc(1, sizeof(edge_list));
    v_arr[3] = calloc(1, sizeof(edge_list));
    edge_t e0 = { v_arr[0], v_arr[1] };
    edge_t e1 = { v_arr[0], v_arr[2] };
    edge_t e2 = { v_arr[1], v_arr[2] };
    edge_t e3 = { v_arr[2], v_arr[3] };
    edge_t e4 = { v_arr[0], v_arr[3] };
    edge_t** edge_arr = malloc(sizeof(edge_t*) * 5);
    edge_arr[0] = &e0; edge_arr[1] = &e1; edge_arr[2] = &e2;
    edge_arr[3] = &e3; edge_arr[4] = &e4;
    
    for (size_t i = 0; i < 5; ++i) {
        append_edge(edge_arr[i]);
    }

    edge_list* next_edge;
    size_t j = 0;
    for (size_t i = 0; i < 4; ++i) {
        j = 0;
        next_edge = v_arr[i];
        while(next_edge->next) {
            next_edge = next_edge->next;
            printf("edge %zu for vertix %zu\n", j++, i) ;
        }
    }

    graph_t g = { v_arr, edge_arr, 5, 4 };
    printf("cut from rgc = %zu\n", random_graph_contraction_trials(1, &g));

    printf("printing graph after collapsing edges\n");
    j = 0;
    for (size_t i = 0; i < 4; ++i) {
        j = 0;
        next_edge = v_arr[i];
        if (!next_edge->next) continue;
        while(next_edge->next) {
            next_edge = next_edge->next;
            printf("edge %zu for vertix %zu\n", j++, i);
        }
    }

    return EXIT_SUCCESS;
}


void append_edge(edge_t* e) {
    edge_list* vi = e->i; 
    edge_list* vj = e->j;

    a_e(vi, e);
    a_e(vj, e);
    return;
}

void a_e(edge_list* v, edge_t* e) {
    edge_list* last = malloc(sizeof(edge_list));
    last->edge = e;
    last->next = 0;

    while (v->next) {
        v = v->next;
    }

    v->next = last;
    return;
}

// recursively free vertice list
void free_vertice(edge_list* vertice_list) {
    if (!vertice_list->next) {
        free(vertice_list);
        return;
    }

    free_vertice(vertice_list->next);

    if (vertice_list->edge) {         // only free vertice list entry
        free(vertice_list);           // if not head of list
    }
}

// remove self referential edges from vertice lists 
void snip_edges(edge_list* vertice) {
    edge_list* last_edge;
    edge_list* current_edge;
    edge_list* next_edge;

    last_edge = vertice;
    current_edge = vertice->next;
    next_edge = current_edge->next;

    while(current_edge) {
        if (current_edge->edge->i == current_edge->edge->j) {
            last_edge->next = next_edge;         // set previous edge to point to next edge
            current_edge->edge->i = 0;           // set edges to null
            current_edge->edge->j = 0;
            free(current_edge);                  // free vertice that points to self referential edge
            current_edge = next_edge;         
            if (current_edge) {
                next_edge = current_edge->next;
            }
        } else {                                 // iterate to next vertice 
            last_edge = current_edge;
            current_edge = current_edge->next;
            if (current_edge) {
                next_edge = current_edge->next;
            }
        }
    }
}

// collapses edge e
void collapse_edge(edge_t* e) {
    edge_list* vi = e->i;
    edge_list* vj = e->j;

    // reroute edges point to i to point to j
    edge_list* next_edge = vi;
    while (next_edge->next) {
        next_edge = next_edge->next;     // move on to next edge in i
        if (next_edge->edge->i == vi) {  // if edge i points to vi
            next_edge->edge->i = vj;     // reroute to vj    
            a_e(vj, next_edge->edge);    // add edge pointing to vj to vj list
        }
        if (next_edge->edge->j == vi) { 
            next_edge->edge->j = vj;
            a_e(vj, next_edge->edge);
        }
    }

    // snip self referential edges from vj
    snip_edges(vj);
    
    // delete vertice vi
    free_vertice(vi);
    vi->edge = 0;
    vi->next = 0;
}

size_t random_graph_contraction(graph_t* g) {
    edge_t** edge_arr = g->edge_arr;
    size_t* edge_check = calloc(g->edge_count, sizeof(size_t));

    for (size_t i = 0; i < g->vertice_count - 2; ++i) {
        size_t random = rand() % g->edge_count;
        if (edge_check[random]) {
            --i;
        } else {
            collapse_edge(edge_arr[random]);
            ++edge_check[random];
        }
    }

    free(edge_check);

    edge_list* current_edge;
    size_t ret = 0;

    for (size_t i = 0; i < g->vertice_count; ++i) {
        current_edge = g->vertice_arr[i];
        if (!current_edge->next) continue;
        while (current_edge->next) {
            ++ret; 
            current_edge = current_edge->next;
        }
        return ret;
    }
    
    return ret;
}

size_t random_graph_contraction_trials(size_t n, graph_t* g) {
    size_t ret = UINT_MAX;
    size_t tmp = 0;

    for (size_t i = 0; i < n; ++i) {
        tmp = random_graph_contraction(g);
        if (tmp < ret) ret = tmp;
    }

    return ret;
}