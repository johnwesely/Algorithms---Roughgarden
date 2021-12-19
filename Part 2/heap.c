#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define L 0
#define R 1

typedef struct __heap {
    size_t*   arr;
    size_t*   last_index;
    size_t*   size;
} heap_t;


heap_t* create_heap() {
    heap_t* ret; = malloc(sizeof(heap));
    ret->arr = calloc(15, sizeof(size_t));
    ret->last_index = 0;
    ret->size = 15;
}

void push_heap(size_t val, heap_t* heap) {
    if (heap->size <= heap->last_index) {
        heap->arr = realloc(heap->arr, sizeof(size_t) * (heap->size + 10));
        heap->size += 10;
    }

    heap->arr[heap->last_index] = val;
    heap_bubble_up(heap, heap->last_index);
}

size_t pop_heap(heap_t* heap) {
    size_t ret = heap->arr[0];
    --heap->size;
    heap->arr[0] = heap->arr[heap->size];
    heap->arr[heap->size] = 0;
    heap_bubble_down(heap, 0);
    return ret;
}

heap_bubble_up(heap_t* heap, size_t i) {
    if (i == 0) return; 
    size_t parent_index = heap_parent_index(heap, i);

    if (heap->arr[parent_index] > heap->arr[i]) {
        size_t tmp = heap->arr[i];
        heap->arr[i] = heap->arr[parent_index];
        heap->arr[parent_index] = tmp;
    } else {
        return;
    }

    heap_bubble_up(heap, parent_index);
}

heap_bubble_down(heap_t* heap, size_t i) {
    if (heap_depth(i) == heap_depth(heap->last_index)) {
        return;
    }

    size_t ci_r = heap_child_index(heap, i, R);
    size_t ci_l = heap_child_index(heap, i, L);
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

size_t heap_parent_index(heap_t* heap, size_t i) {

}

size_t heap_child_index(size_t* heap, size_t i, size_t dir) {

}

size_t heap_depth(size_t i) {

}