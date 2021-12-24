#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MIN 0
#define MAX 1
#define L 0
#define R 1

typedef struct __heap {
    size_t* arr;
    size_t last_index;
    size_t type;
} heap_t;

typedef struct __median_maintenence_heap {
    heap_t* low;
    heap_t* high;
    size_t median;
} mm_heap_t;

heap_t* create_heap(size_t size, size_t type);
void destroy_heap(heap_t* heap);
void push_heap(size_t n, heap_t* heap);
size_t pop_heap(heap_t* heap);
mm_heap_t* create_mm_heap(size_t size);
void push_mm_heap(size_t n, mm_heap_t* mm_heap); 
void heap_bubble_up(heap_t* heap, size_t i);
void heap_bubble_down(heap_t* heap, size_t i);
void swap_heap_val(size_t* a, size_t* b);
size_t heap_parent_index(size_t i);
size_t heap_child_index(size_t i, size_t dir);
size_t heap_depth(size_t i);

int main(int argc, char* argv[]) {
    return EXIT_SUCCESS;
}

void push_mm_heap(size_t n, mm_heap_t* mm_heap) {
    if (mm_heap->high->last_index == 0 && mm_heap->low->last_index == 0) {
        mm_heap->median = n;
        return;
    }

    if (mm_heap->high->last_index == mm_heap->low->last_index) {
        if (n < mm_heap->median) {
            push_heap(n, mm_heap->low);
        } else {
            push_heap(n, mm_heap->high);
        }
        return;
    }

    if (mm_heap->high->last_index > mm_heap->low->last_index) {
        if (n > mm_heap->median) {
            push_heap(mm_heap->median, mm_heap->low);
            push_heap(n, mm_heap->high);
            mm_heap->median = pop_heap(mm_heap->high);
        } else {
            push_heap(mm_heap->median, mm_heap->high);
            push_heap(n, mm_heap->low);
            mm_heap->median = pop_heap(mm_heap->high);
        }
    } else {
        if (n > mm_heap->median) {
            push_heap(mm_heap->median, mm_heap->low);
            push_heap(n, mm_heap->high);
            mm_heap->median = pop_heap(mm_heap->low);
        } else {
            push_heap(mm_heap->median, mm_heap->high);
            push_heap(n, mm_heap->low);
            mm_heap->median = pop_heap(mm_heap->low);
        }
    }
}

heap_t* create_heap(size_t size, size_t type) {
    heap_t* ret = malloc(sizeof(heap_t));
    ret->arr = calloc(size, sizeof(size_t));
    ret->last_index = 0;
    ret->type = type;
}

mm_heap_t* creat_mm_heap(size_t size) {
    mm_heap_t* ret = malloc(sizeof(mm_heap_t));

    ret->median = 0;

    ret->high = malloc(sizeof(heap_t));
    ret->high->arr = calloc(size, sizeof(size_t));
    ret->high->last_index = 0;
    ret->high->type = MIN;

    ret->low = malloc(sizeof(heap_t));
    ret->low->arr = calloc(size, sizeof(size_t));
    ret->low->last_index = 0;
    ret->low->type = MAX;

    return ret;
}

void destroy_heap(heap_t* heap) {
    free(heap->arr);
    free(heap);
}

void push_heap(size_t n, heap_t* heap) {
    heap->arr[heap->last_index] = n;
    heap_bubble_up(heap, heap->last_index);
    ++heap->last_index;
}

size_t pop_heap(heap_t* heap) {
    if (!heap->last_index) return UINT_MAX;

    size_t ret = heap->arr[0];
    --heap->last_index;
    heap->arr[0] = heap->arr[heap->last_index];
    heap->arr[heap->last_index] = 0;

    heap_bubble_down(heap, 0);
}

void heap_bubble_up(heap_t* heap, size_t i) {
    if (i == 0) return;
    size_t parent_index = heap_parent_index(i);

    if (heap->type) {
        if (heap->arr[i] > heap->arr[parent_index]) {
            swap_heap_val(&heap->arr[i], &heap->arr[parent_index]);
        } else {
            return;
        }
    } else {
        if (heap->arr[i] < heap->arr[parent_index]) {
            swap_heap_val(&heap->arr[i], &heap->arr[parent_index]);
        } else {
            return;
        }
    }

    heap_bubble_up(heap, parent_index);
}

void heap_bubble_down(heap_t* heap, size_t i) {
    if (heap_depth(i) == heap_depth(heap->last_index)) return;

    size_t ci_r = heap_child_index(i, R);
    size_t ci_l = heap_child_index(i, L);
    size_t chosen_ci = 0;

    if (heap->type) {
        chosen_ci = (heap->arr[ci_r] > heap->arr[ci_l]) ? ci_r : ci_l;
    } else {
        chosen_ci = (heap->arr[ci_r] < heap->arr[ci_r]) ? ci_r : ci_l;
    } 

    if (heap->type) {
        if (heap->arr[i] < heap->arr[chosen_ci]) {
            swap_heap_val(&heap->arr[i], &heap->arr[chosen_ci]);
        } else {
            return;
        }
    } else {
        if (heap->arr[i] > heap->arr[chosen_ci]) {
            swap_heap_val(&heap->arr[i], &heap->arr[chosen_ci]);
        } else {
            return;
        }
    }
    
    heap_bubble_down(heap, chosen_ci);
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
