#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    heap_t* heap = create_heap();

    for (size_t i = 0; i < 15; ++i) {
        push_heap(i * 2, heap);
    }

    print_heap(heap);

    for (size_t i = 0; i < 6; ++i) {
        printf("popping %zu off heap\n", pop_heap(heap));
    }

    print_heap(heap);
    destroy_heap(heap);
}
