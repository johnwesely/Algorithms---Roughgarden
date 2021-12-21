#include <stdio.h>
#include <stdlib.h>

#ifndef HEAP_H
#define HEAP_H

// id weight pairing for priority queue 
typedef struct __dij_pair {
    size_t   id;
    size_t   weight; 
} dij_pair_t;

// heap data structure
typedef struct __heap {
    dij_pair_t*  arr;
    size_t       last_index;
    size_t       size;
} heap_t;

// heap API
void print_heap(heap_t* heap);
heap_t* create_heap();
void destroy_heap(heap_t* heap);
void push_heap(dij_pair_t dp, heap_t* heap);
size_t pop_heap(heap_t* heap);

#endif