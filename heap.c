#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "heap.h"

#define L 1
#define R 2


void heap_bubble_up(heap_t* heap, size_t i);
void heap_bubble_down(heap_t* heap, size_t i);
size_t heap_parent_index(size_t i);
size_t heap_child_index(size_t i, size_t dir);
size_t heap_depth(size_t i);


heap_t* create_heap() {
    heap_t* ret = malloc(sizeof(heap_t));
    ret->arr = malloc(15 * sizeof(size_t));

    for (size_t i = 0; i < 15; ++i) {
        ret->arr[i] = UINT_MAX;
    }

    ret->last_index = 0;
    ret->size = 15;
    return ret;
}

void destroy_heap(heap_t* heap) {
    free(heap->arr);
    free(heap);
}

void push_heap(size_t val, heap_t* heap) {
    if (heap->size <= heap->last_index) {
        size_t old_size = heap->size;
        heap->arr = realloc(heap->arr, sizeof(size_t) * (heap->size + 10));
        heap->size += 10;
        
        for (size_t i = 0; i < 10; ++i) {
            heap->arr[old_size + i] = UINT_MAX;
        }
    }

    heap->arr[heap->last_index] = val;
    heap_bubble_up(heap, heap->last_index);
    ++heap->last_index;
}

size_t pop_heap(heap_t* heap) {
    size_t ret = heap->arr[0];
    --heap->last_index;
    heap->arr[0] = heap->arr[heap->last_index];
    heap->arr[heap->last_index] = UINT_MAX;
    heap_bubble_down(heap, 0);
    return ret;
}

void heap_bubble_up(heap_t* heap, size_t i) {
    if (i == 0) return; 
    size_t parent_index = heap_parent_index(i);

    if (heap->arr[parent_index] > heap->arr[i]) {
        size_t tmp = heap->arr[i];
        heap->arr[i] = heap->arr[parent_index];
        heap->arr[parent_index] = tmp;
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
    size_t smaller_ci = (heap->arr[ci_r] < heap->arr[ci_l]) ? ci_r : ci_l;
    
    if (heap->arr[i] > heap->arr[smaller_ci]) {
        size_t tmp = heap->arr[i];
        heap->arr[i] = heap->arr[smaller_ci];
        heap->arr[smaller_ci] = tmp;
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
        printf("%zu == %zu  ", i, heap->arr[i]);
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
