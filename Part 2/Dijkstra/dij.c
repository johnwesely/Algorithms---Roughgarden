#include "heap.h"
#include "weighted-undirected-graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

size_t* dijkstra_shortest_distance(size_t s, graph_t* g);
void dij(size_t v, graph_t* g, size_t* distances, size_t* visited);
size_t* dij_naive(size_t s, graph_t* g);
size_t pick_next(size_t* distances, size_t* visited, size_t v_count);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("include a file dingus\n");
        return EXIT_FAILURE;
    }

    FILE* f = fopen(argv[1], "r");

    if (!f) {
        perror("failure to load file ");
        return EXIT_FAILURE;
    }

    graph_t* g = read_graph(f);
    print_graph(g);

    size_t* distances = dijkstra_shortest_distance(0, g);
    for (size_t i = 0; i < g->v_count; ++i) {
        printf("%zu  %zu\n", i + 1, distances[i]);
    }

    size_t print_arr[] = { 7, 37, 59, 82, 99, 115, 133, 165, 188, 197 };

    printf("chosen vertices are\n");
    for (size_t i = 0; i < 10; ++i) {
        printf("%zu,", distances[print_arr[i] - 1]);
    }
    printf("\n");

    free(distances);
    destroy_graph(g);
    fclose(f);

    return EXIT_SUCCESS;
}

// O(m log n) implementation of dijkstras using a hash mapped heap
size_t* dijkstra_shortest_distance(size_t s, graph_t* g) {
    size_t x_size = 0;                                        // number of vertices in set of visited vertices
    size_t* distances = malloc(g->v_count * sizeof(size_t));  // distances from vertice s
    size_t* visited = calloc(g->v_count, sizeof(size_t));
    heap_t* pq = create_heap(g->v_count);

    for (size_t i = 0; i < g->v_count; ++i) {                 // populate distances with infinity
        distances[i] = UINT_MAX;
    }

    distances[s] = 0;                        // set distance of s to zero
    ++visited[s];                            // add s to set of visted vertices
    ++x_size;                                // increment size of visted vertices set
    edge_t* current = g->vert_arr[s].head;   // set s to current vertice
    size_t dgs = UINT_MAX;                   // Dijkstra's Greedy Criterion
    size_t current_distance = 0;             // distance of current vertice from s
    size_t next = 0;                         // unvisted vertice that has the closest calculated distance from s

    while(x_size < g->v_count) {
        while (current) {
            dgs = current_distance + current->weight;                                     // calculate dgs for current
            if (dgs < distances[current->id] && pq->heap_map[current->id] == UINT_MAX) {  // if current is smaller than distances[current] is not in heap  
                distances[current->id] = dgs;
                dij_pair_t dp =  { current->id, dgs };
                push_heap(dp, pq);                                                        // push current to heap
            } 
            if (dgs < distances[current->id] && pq->heap_map[current->id] != UINT_MAX) {  // if current is smaller than distances[current] and is in heap
                distances[current->id] = dgs;
                decrease_heap_weight(pq, current->id, dgs);                               // decrease weight of current within heap 
            }
            current = current->next;
        }

            next = pop_heap(pq);                    // pop winner of Dijkstra's Greedy Criteria off heap
            current = g->vert_arr[next].head;       // add winner to list of visted vertexs and continue
            current_distance = distances[next];
            ++visited[next];
            ++x_size;
    }

        free(visited);
        destroy_heap(pq);
        return distances;
}

// naive implementation of algorithm with running time of O(N squared)
size_t* dij_naive(size_t s, graph_t* g) {
    size_t x_size = 0;                                        // number of vertices in set of visted vertices
    size_t* distances = malloc(g->v_count * sizeof(size_t));  // distance for all vertices in g from s
    size_t* visited = calloc(g->v_count, sizeof(size_t));     // set of visted vertices

    for (size_t i = 0; i < g->v_count; ++i) {                 // set each distance from s to infinity
        distances[i] = UINT_MAX;
    }

    distances[s] = 0;                        // set distance of s to zero
    ++visited[s];                            // add s to set of visted vertices
    ++x_size;                                // increment size of visted vertices set
    edge_t* current = g->vert_arr[s].head;   // set s to current vertice
    size_t dgs = UINT_MAX;                   // Dijkstra's Greedy Criterion
    size_t current_distance = 0;             // distance of current vertice from s
    size_t next = 0;                         // unvisted vertice that has the closest calculated distance from s

    while (x_size < g->v_count) {                       // while number of vertices in visited set is less than total
        while (current) {                               // while current vertice still has outgoing edges
            dgs = current_distance + current->weight;   // calculate dgs, distance to current vertice plus distance to outgoing vertice
            if (dgs < distances[current->id]) {         // if dgs is less than previously calculated value or infinty
                distances[current->id] = dgs;           // set distance of outgoing vertice to dgs
            }
            current = current->next;                    // move on to next vertice pointed to by current
        }

        next = pick_next(distances, visited, g->v_count);   // pick unvisted vertice with smallest distance from s
        current = g->vert_arr[next].head;                   // set current to that vertice
        current_distance = distances[next];                 
        ++visited[next];                                    // add new vertice to set of visted vertices
        ++x_size;
    }

    free(visited);
    return distances;   // after cycling through every edge distance to every vertice will be correctly calcuated                                  
}

// picks next vertice for naived dijkstra, running time proportional to O(N) 
size_t pick_next(size_t* distances, size_t* visited, size_t v_count) {
    size_t min = UINT_MAX;
    size_t min_index = 0;

    for (size_t i = 0; i < v_count; ++i) {
        if (!visited[i] && distances[i] < min) {
            min_index = i;
            min = distances[i];
        }
    }

    return min_index;
}

