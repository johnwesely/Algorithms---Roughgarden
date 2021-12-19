#include "heap.c"

// heap data structure
typedef struct __heap {
    size_t*   arr;
    size_t*   last_index;
    size_t*   size;
} heap_t;

// heap API
void print_heap(heap_t* heap);
heap_t* create_heap();
void destroy_heap(heap_t* heap);
void push_heap(size_t val, heap_t* heap);
size_t pop_heap(heap_t* heap);