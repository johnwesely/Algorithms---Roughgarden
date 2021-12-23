#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "heap.h"

#define L 1
#define R 2

void heap_bubble_up(heap_t* heap, size_t i);                // bubbles dij pair at index i up heap 
void heap_bubble_down(heap_t* heap, size_t i);              // bubbles dij pair at index i down heap
size_t heap_parent_index(size_t i);                         // returns parent index of heap index i
size_t heap_child_index(size_t i, size_t dir);              // returns child index of heap index i, pass L for left child, R for right
size_t heap_depth(size_t i);                                // returns depth of heap entry at index i
void swap_dij_pair(dij_pair_t* a, dij_pair_t* b);           // swaps dij_pair in heap
void swap_heap_map(heap_t* heap, size_t id_i, size_t id_j);  // swaps locations of ids i and j in heap map
void print_heap_map(heap_t* heap);                            // prints heap_map for debugging
size_t check_heap_map(heap_t* heap, char* calling_fn);

size_t global = 0;

// allocates heap and returns pointer to heap
heap_t* create_heap(size_t v_count) {
    heap_t* ret = malloc(sizeof(heap_t));
    ret->arr = malloc(v_count * 2 * sizeof(dij_pair_t));

    for (size_t i = 0; i < v_count * 2; ++i) {
        ret->arr[i].id = 0;
        ret->arr[i].weight = UINT_MAX;
    }

    ret->heap_map = malloc(v_count * sizeof(size_t));

    for (size_t i = 0; i < v_count; ++i) {
        ret->heap_map[i] = UINT_MAX;
    }

    ret->last_index = 0;
    ret->size = 128;
    return ret;
}

// frees heap
void destroy_heap(heap_t* heap) {
    free(heap->arr);
    free(heap->heap_map);
    free(heap);
}

void push_heap(dij_pair_t dp, heap_t* heap) {

    heap->arr[heap->last_index].id = dp.id;          // place dp at bottom of heap
    heap->arr[heap->last_index].weight = dp.weight;
    heap->heap_map[dp.id] = heap->last_index;        // update dp.id in hash map
    heap_bubble_up(heap, heap->last_index);          // bubble dp up to appropriate location in heap
    ++heap->last_index;
}

size_t pop_heap(heap_t* heap) {
    if (!heap->last_index) {  // return infinity if heap is empty
        return UINT_MAX;
    }

    size_t ret = heap->arr[0].id;   
    heap->heap_map[heap->arr[0].id] = UINT_MAX;  // set heap map of id being popped to infinity
    --heap->last_index;
    heap->arr[0] = heap->arr[heap->last_index];  // place last entry in heap at head
    heap->heap_map[heap->arr[0].id] = 0;         // update heap->arr[0].id in heap map

    heap->arr[heap->last_index].id = 0;
    heap->arr[heap->last_index].weight = UINT_MAX;

    heap_bubble_down(heap, 0);                   // bubble down to appropriate location
    return ret;
}

void heap_bubble_up(heap_t* heap, size_t i) {
    if (i == 0) return; 
    size_t parent_index = heap_parent_index(i);

    if (heap->arr[parent_index].weight > heap->arr[i].weight) {
        swap_dij_pair(&heap->arr[i], &heap->arr[parent_index]);
        swap_heap_map(heap, heap->arr[i].id, heap->arr[parent_index].id);  // maintain heap map
        check_heap_map(heap, "bu after calling shm");
    } else {
        return;
    }
    heap_bubble_up(heap, parent_index);
}

void heap_bubble_down(heap_t* heap, size_t i) {
    if (heap_depth(i) == heap_depth(heap->last_index)) {
        return;
    }

    size_t ci_r = heap_child_index(i, R);
    size_t ci_l = heap_child_index(i, L);
    size_t smaller_ci = 0;
    smaller_ci = (heap->arr[ci_r].weight < heap->arr[ci_l].weight) ? ci_r : ci_l;
    
    if (heap->arr[i].weight > heap->arr[smaller_ci].weight) {
        swap_dij_pair(&heap->arr[i], &heap->arr[smaller_ci]);
        swap_heap_map(heap, heap->arr[i].id, heap->arr[smaller_ci].id);   // maintain heap map
    } else {
        return;
    }

    heap_bubble_down(heap, smaller_ci);
}

void print_heap(heap_t* heap) {
    size_t current_depth = 0;
    for (size_t i = 0; i < heap->last_index; ++i) {
        if (current_depth < heap_depth(i)) {
            printf("\n");
            ++current_depth;
        }
        printf("%zu == id = %zu, weight = %zu\n", i, heap->arr[i].id, heap->arr[i].weight);
    }
    printf("\n");
}

size_t heap_parent_index(size_t i) {
    return (i - 1) / 2;
}

size_t heap_child_index(size_t i, size_t dir) {
    return (i * 2) + dir;
}

size_t heap_depth(size_t i) {
    size_t depth = 0;
    ++i;
    
    while(i > 1) {
        i /= 2;
        ++depth;
    }

    return depth;
}

void swap_dij_pair(dij_pair_t* a, dij_pair_t* b) {
    dij_pair_t tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_heap_map(heap_t* heap, size_t id_i, size_t id_j) {
    size_t tmp = heap->heap_map[id_i];
    heap->heap_map[id_i] = heap->heap_map[id_j];
    heap->heap_map[id_j] = tmp;
}

void decrease_heap_weight(heap_t* heap, size_t id, size_t new_weight) {
    size_t heap_index = heap->heap_map[id];
    if (heap_index > heap->last_index) {
    }

    if (new_weight < heap->arr[heap_index].weight) {
        heap->arr[heap_index].weight = new_weight;
        heap_bubble_up(heap, heap_index);
    } else {
        printf("descrease_heap_weight presented with new weight larger than previous weight\n");
    }
}

void print_heap_map(heap_t* heap) {
    for (size_t i = 0; i < 200; ++i) {
        printf("id %zu = heap->arr[%zu]\n", i, heap->heap_map[i]);
    }
}

size_t check_heap_map(heap_t* heap, char* calling_fn) {
    for (size_t i = 0; i < 200; ++i) {
        if (heap->heap_map[i] > heap->last_index + 1 && heap->heap_map[i] != UINT_MAX) {
            printf("heap map invariant broken by %s\n", calling_fn);
            if (!global) {
                print_heap_map(heap);
                print_heap(heap);
                ++global;
            }
            return i;
        }
    }
    return 0;
}
