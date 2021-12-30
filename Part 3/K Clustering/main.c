#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "union-find.h"
#include "weighted-undirected-graph.h"

graph_t* read_graph(FILE* f);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("K-Cluster takes 1 command line argument\n");
        return EXIT_FAILURE;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("cannot open file ");
        return EXIT_FAILURE;
    }

    graph_t* g = read_graph(f);
    print_graph(g);

    fclose(f);
    destroy_graph(g);

    return EXIT_SUCCESS;
}

size_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream);

size_t read_edge_count(FILE* f) {
    fseek(f, 0, SEEK_SET);
    size_t e_count = 0;

    size_t buff_size = 256;
    char* buff = calloc(buff_size, 1);

    getline(&buff, &buff_size, f);

    while (getline(&buff, &buff_size, f) != -1) {
        ++e_count;
    }

    free(buff);
    fseek(f, 0, SEEK_SET);
    return e_count;
}

graph_t* read_graph(FILE* f) {
    fseek(f, 0, SEEK_SET);
    size_t e_count = read_edge_count(f);

    size_t buff_size = 256;
    char* buff = calloc(buff_size, 1);

    if (getline(&buff, &buff_size, f) == -1) {
        printf("empty_file\n");
        exit(0);
    }

    graph_t* g = create_graph(atoi(buff), e_count);
    
    size_t i = 0; size_t j = 0; size_t w = 0; 
    size_t edge_index = 0;

    while (getline(&buff, &buff_size, f) != -1) {
        i = atoi(strtok(buff, " ")) - 1;
        j = atoi(strtok(0, " ")) - 1;
        w = atoi(strtok(0, " "));
        append_edge(g, i, j, w, edge_index++);
    }

    free(buff);
    return g;
}