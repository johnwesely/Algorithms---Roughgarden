/* implementation of random contraction algorithm for an undirected graph
** takes two command line arguments, a file representing a graph and a number of trials to run
** format of graph file is as follows:
**     1 2 3 4
**     2 1 3
**     3 1 2 4
**     4 1 3
** first number in every row is a node, following numbers in row are all nodes pointed to by node
*/

typedef struct __edge_list edge_list;
typedef struct __edge edge_t; 

// a vertex is a linked list of edges
struct __edge_list {
    edge_t* edge;
    edge_list* next;
}; 

// an edge points to vertices
struct __edge {
    edge_list* i;    // pointers to edges
    edge_list* j;
    size_t     ii;   // user readable indices for printing
    size_t     ji;
};

// A graph is an array of vertices, an array of edges pointing to vertices, and member counts of both
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
graph_t* read_graph(FILE* f);
size_t count_vertices(FILE* f);
void check_vertice_arr(graph_t* g);
void check_edges(edge_t** edge_arr);
void free_graph(graph_t*g);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Randomized Contraction takes two command line arguments, a file containing a graph, and the number of trials to run\n");
        return EXIT_FAILURE;
    }
    srand(1);
    srand(time(0));

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("unable to open file: ");
        return EXIT_FAILURE;
    }

    int n = atoi(argv[2]);
    size_t min_cut = UINT_MAX;
    size_t cut = 0;

    for (int i = 0; i < n; ++i) {
        graph_t* g = read_graph(f);
        //check_vertice_arr(g);
        cut = random_graph_contraction(g);
        if (cut < min_cut) min_cut = cut;
        free_graph(g);
    }

    printf("Minimum Cut from %d trials is %zu\n", n, min_cut);

    return EXIT_SUCCESS;
}

// append edge to vertice list
void append_edge(edge_t* e) {
    edge_list* vi = e->i; 
    edge_list* vj = e->j;

    a_e(vi, e);
    a_e(vj, e);
    return;
}

// append edge helper function
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

void collapse_edge(edge_t* e) {
    //printf("collasping edge { %zu, %zu }\n", e->ii, e->ji);
    if (e->ii == e->ji) {
        printf("e->ii = %zu, e->ji = %zu\n", e->ii, e->ji);
        printf("found failed snip!\n");
        return;
    }
    edge_list* vi = e->i;
    edge_list* vj = e->j;

    // reroute edges pointing to i to point to j
    edge_list* current_edge = vi;
    while (current_edge->next) {
        current_edge = current_edge->next;     // move on to next edge in i
        if (!current_edge->edge) continue;
        if (current_edge->edge->i == vi) {     // if edge i points to vi
            if (current_edge->edge->j == vj) {  // if self reference would be created 
                current_edge->edge->i = 0;     // set edge to zero
                current_edge->edge->j = 0;
                current_edge->edge->ii = 0;
                current_edge->edge->ji = 0;
                current_edge->edge = 0;
                continue;
            } else {                               // otherwise
                current_edge->edge->i = vj;        // reroute to vj    
                current_edge->edge->ii = e->ji;
                a_e(vj, current_edge->edge);       // add edge pointing to vj to vj list
            }
        }
        if (current_edge->edge->j == vi) {     // if edge j points to vi
            if (current_edge->edge->i == vj) {  // if self refrence would be created
                current_edge->edge->j = 0;     // set edge to zero
                current_edge->edge->i = 0;
                current_edge->edge->ii = 0;
                current_edge->edge->ji = 0;
                current_edge->edge = 0;
                continue;
            } else {                               // otherwise
                current_edge->edge->j = vj;        // reroute to vj
                current_edge->edge->ji = e->ji;
                a_e(vj, current_edge->edge);
            }
        }
    }

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
            //check_vertice_arr(g);
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
            if (current_edge->edge) ++ret; 
            current_edge = current_edge->next;
        }
        return ret;
    }
    
    return ret;
}

graph_t* read_graph(FILE* f) {
    fseek(f, 0, SEEK_SET);
    size_t buff_size = 4096;
    char* line_buff = calloc(buff_size, 1);
    graph_t* g = malloc(sizeof(graph_t));   
    size_t vertice_c = count_vertices(f);  // count vertices
    fseek(f, 0 , SEEK_SET);
    size_t edge_c = 0;                     // counter for number of edges
    // 2d array for checking if edges is already present in edge array
    size_t** edge_check = malloc(sizeof(size_t*) * vertice_c); 
    for (size_t i = 0; i < vertice_c; ++i) {
        edge_check[i] = calloc(vertice_c, sizeof(size_t));
    }
     
    // Initialize vertice array
    edge_list** vert_arr = calloc(vertice_c, sizeof(edge_list*));
    // add headers to ever vertice in vertice array 
    for (size_t i = 0; i < vertice_c; ++i) {
        vert_arr[i] = calloc(1, sizeof(edge_list));
    }
    // edge array
    edge_t** edge_arr = calloc(16384, sizeof(edge_t));

    size_t i = 0;                  // indices
    size_t vi = 0; size_t vj = 0;
    char buff[256];                // int buff
    size_t bi = 0;                 // int buff index
    int line_len;
    edge_t* current_edge;
    while ((line_len = getline(&line_buff, &buff_size, f)) > 0) {
        i = 0;
        bi = 0;
        while (line_buff[i] < 58 && line_buff[i] > 47) {    // read vertice index to buff
            buff[bi++] = line_buff[i++];
        }

        buff[bi] = 0;
        vi = atoi(buff) - 1;                                // vertice index

        while (i < line_len - 1) {                          // read vjs until end of lilne
            bi = 0;
            while(line_buff[i] > 57 || line_buff[i] < 48) { // skip non numeric chars
                ++i;
            }

            while(line_buff[i] < 58 && line_buff[i] > 47 && i < line_len) {  // read vj into buff
                buff[bi++] = line_buff[i++];
            }
            
            buff[bi] = 0;

            if (buff[0] != '\n') {
                vj = atoi(buff) - 1;
                if (!edge_check[vi][vj] && !edge_check[vj][vi]) {  // check to make sure edge was not added previously
                    current_edge = malloc(sizeof(edge_t));
                    current_edge->i = vert_arr[vi];                // populate edge with data
                    current_edge->j = vert_arr[vj];
                    current_edge->ii = vi + 1;
                    current_edge->ji = vj + 1;
                    edge_arr[edge_c] = current_edge;
                    append_edge(edge_arr[edge_c++]);               // add edge to appropriate vertice lists
                    edge_check[vi][vj]++;                          // note edge as added to graph
                    edge_check[vj][vi]++;
                } else {
                    edge_check[vi][vj]--;                          // account for parallel edges
                    edge_check[vi][vj]--;
                }

            }
        }
    }

    g->vertice_arr = vert_arr;
    g->edge_arr = edge_arr;
    g->edge_count = edge_c;
    g->vertice_count = vertice_c;

    for (size_t i = 0; i < vertice_c; ++i) {    // free alloc'd memory
        free(edge_check[i]);
    }
    free(edge_check);
    free(line_buff);
    return g;
}

size_t count_vertices(FILE* f) {
    fseek(f, 0, SEEK_SET);
    size_t buff_size = 256;
    char* buff = malloc(buff_size);
    if (!buff) {
        perror("allocation for count vertices buffer failed ");
        return 0;
    }

    size_t line_count = 0;

    while (getline(&buff, &buff_size, f) > 0) {
        ++line_count;
    }

    free(buff);
    return line_count;
}

// prints all edgelists in vertice array for testing
void check_vertice_arr(graph_t* g) {
    for (size_t i = 0; i < g->vertice_count; ++i) {
        edge_list* current = g->vertice_arr[i];
        size_t vertice_c = 0;
        while (current->next) {
            vertice_c++;
            current = current->next;
        }
        if (vertice_c) printf("vertice %zu has %zu edges\n", i + 1, vertice_c);
    }
}

// free graph
void free_graph(graph_t* g) {
    for (size_t i = 0; i < g->vertice_count; ++i) {
        if (g->vertice_arr[i]) {
            free_vertice(g->vertice_arr[i]);
            free(g->vertice_arr[i]);
        }
    }
    free(g->vertice_arr);

    for (size_t i = 0; i < g->edge_count; ++i) {
        free(g->edge_arr[i]);
    }

    free(g->edge_arr);
    free(g);
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