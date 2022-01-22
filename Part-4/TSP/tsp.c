#include "tsp.h"

void setup(graph_t g, double** memo, size_t n);
void solve(graph_t g, double** memo, size_t n); 
double min_cost(graph_t g, double** memo, size_t n);
size_t not_in_subset(size_t v, size_t sub_set); 
subset_list_t* generate_combinations(size_t gen_n, size_t n);
void gc(size_t subset, size_t at, size_t gen_n, size_t n, subset_list_t* subsets);
void append_subset_list(subset_list_t* subsets, size_t subset);
void destroy_subset_list(subset_list_t* subsets);
void destroy_subset_nodes(subset_node_t* subsets);
void print_memo_table(double** memo, size_t n);
void print_graph(graph_t g, size_t n);


double find_shortest_tour(graph_t g, size_t n) {
    double** memo = malloc(sizeof(double*) * n);       // create n by 2^n memo table
    size_t mt_depth = pow(2, n);
    for (size_t i = 0; i < n; ++i) {
        memo[i] = calloc(mt_depth, sizeof(double));
    }

    setup(g, memo, n);                               // initialize memo table
    solve(g, memo, n);                               // populate memo table

    double ret = min_cost(g, memo, n);

    
    for (size_t i = 0; i < n; ++i) {
        free(memo[i]);
    }
    free(memo);

    return ret;
}

void setup(graph_t g, double** memo, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        memo[i][1 | 1 << i] = g[0][i];     // set bit mask representing each intial hope to distance from zero to i
    }
}

void solve(graph_t g, double** memo, size_t n) {
    for (size_t gen = 3; gen < n + 1; ++ gen) {                        // for generations 3 through n-1
        subset_list_t* subset_list = generate_combinations(gen, n);
        subset_node_t* subset = subset_list->head;

        while (subset) {                                           // for each possible subset of generation members
            if (not_in_subset(0, subset->set)) {                   // if 0th bit is not set, move to next subset
                subset = subset->next;
                continue;
            }

            for (size_t next = 1; next < n; ++next) {              // for each possible next vertice
                if (not_in_subset(next, subset->set)) continue;
                size_t state = subset->set ^ (1 << next);          // subset without next hop
                double minimum_distance = 9000000;

                for (size_t last_j = 1; last_j < n; ++last_j) {                               // for each possible last hop
                    if (last_j == next || not_in_subset(last_j, subset->set)) continue;       // if possible last hop is not in subset, skip
                    float candidate_distance = memo[last_j][state] + g[last_j][next];         // calculate distance of mimumum length of path ending with last hop
                    if (candidate_distance < minimum_distance) {                              // if that distance is smaller the current minumum, set current minimum 
                        minimum_distance = candidate_distance;
                    }
                }
                memo[next][subset->set] = minimum_distance;   // add this iterations minimum to appropriate location in memo table
            }
            subset = subset->next;
        }
        destroy_subset_list(subset_list);
    }
}

double min_cost(graph_t g, double** memo, size_t n) {
    size_t end_state = (1 << n) - 1;

    double min_tour_cost = 9000000;

    for (size_t i = 1; i < n; ++i) {
        double candidate_cost = memo[i][end_state] + g[i][0];  // cost of shortest path containing all nodes to i with final hop back to 0
        if (candidate_cost < min_tour_cost) {
            min_tour_cost = candidate_cost;
        }
    }

    return min_tour_cost;
}

// recursively generate all combinations with gen_n flipped bits out of n total bits
subset_list_t* generate_combinations(size_t gen_n, size_t n) {
    subset_list_t* subsets = malloc(sizeof(subset_list_t)); 
    subsets->head = 0;
    gc(0, 0, gen_n, n, subsets);
    return subsets;
}

// recursive helper function for generate combinations 
void gc(size_t subset, size_t at, size_t gen_n, size_t n, subset_list_t* subsets) {
    if (gen_n == 0) append_subset_list(subsets, subset);

    for (size_t i = at; i < n; ++i) {
        subset = subset | (1 << i);
        gc(subset, i + 1, gen_n - 1, n, subsets);
        subset = subset & ~(1 << i);
    }
}

// functions for managing subset lists
void append_subset_list(subset_list_t* subsets, size_t subset) {
    subset_node_t* subset_node = malloc(sizeof(subset_node_t));
    subset_node->set = subset;
    subset_node->next = subsets->head;
    subsets->head = subset_node;
}

void destroy_subset_list(subset_list_t* subsets) {
    destroy_subset_nodes(subsets->head);
    free(subsets);
}

void destroy_subset_nodes(subset_node_t* subsets) {
    if (!subsets) {
        return;
    }

    destroy_subset_nodes(subsets->next);

    free(subsets);
}

size_t not_in_subset(size_t v, size_t sub_set) {  // returns true is vertex v is not in sub_set
    return (((1 << v) & sub_set) == 0);
}

// printing functions for debuggging
void print_memo_table(double** memo, size_t n) {
    size_t mt_depth = pow(2, n);

    for (size_t i = 0; i < mt_depth; ++i) {
        for (size_t j = 0; j < n; ++j) {
            printf("%lf ", memo[j][i]);
        }
        printf("\n");
    }
}

void print_graph(graph_t g, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            printf("distance from %zu to %zu is %lf\n", i, j, g[i][j]);
        }
    }
}