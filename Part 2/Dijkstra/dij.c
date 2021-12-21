#include "heap.h"
#include "weighted-undirected-graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    heap_t* heap = create_heap();

    for (size_t i = 0; i < 7; ++i) {
        size_t random = rand() % 100;
        printf("pushing %zu onto heap\n", random);
        push_heap(random, heap);
    }

    print_heap(heap);

    for (size_t i = 0; i < 3; ++i) {
        printf("popping %zu off heap\n", pop_heap(heap));
        print_heap(heap);
    }

    for (size_t i = 0; i < 5; ++i) {
        size_t random = rand() % 100;
        printf("pushing %zu onto heap\n", random);
        push_heap(random, heap);
    }

    for (size_t i = 0; i < 9; ++i) {
        printf("popping %zu off heap\n", pop_heap(heap));
    }

    print_heap(heap);
    destroy_heap(heap);
}
