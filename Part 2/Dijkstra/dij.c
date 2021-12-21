#include "heap.h"
#include "weighted-undirected-graph.h"
#include <stdio.h>
#include <stdlib.h>

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
    destroy_graph(g);

    return EXIT_SUCCESS;
}
