#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef TSP_H
#define TSP_H

typedef struct __coordinate {
    double x;
    double y;
} coordinate_t;

typedef struct __subset_node subset_node_t;
struct __subset_node {
    size_t         set;
    subset_node_t* next;
};

typedef struct __subset_list {
    subset_node_t* head;
} subset_list_t;

typedef double** graph_t;

double find_shortest_tour(graph_t g, size_t n);

#endif