#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "wdg.h"
#include "floyd-warshall.h"

int shortest_shortest_path(int** distances, graph_t* g); 
graph_t* read_graph(FILE* f);
    
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Huffman takes one command line arugments");
        return EXIT_FAILURE;
    } 

    size_t buff_size = 256;
    char* path_buff = malloc(buff_size);
    getcwd(path_buff, 256);
    char* test = malloc(buff_size);
    strcpy(test, "/Tests");
    path_buff = strcat(path_buff, test);

    if (chdir(path_buff) == -1) {
        printf("%s\n", path_buff);
        perror("test folder could not be opened ");
        return EXIT_FAILURE;
    }

    FILE* f = fopen(argv[1], "r");

    if (!f) {
        perror("file could not be opened ");
        return EXIT_FAILURE;
    }

    graph_t* g = read_graph(f);
    int** distances = floyd_warshall(g);

    if (distances) {
        printf("shortest shortest path path is length %d\n", shortest_shortest_path(distances, g));
        destroy_distances(distances, g);
    }

    destroy_graph(g);
    fclose(f);

    return EXIT_SUCCESS;
}

graph_t* read_graph(FILE* f) {
    fseek(f, 0, SEEK_SET);

    size_t buff_size = 256;
    char* buff = calloc(buff_size, 1);

    if (getline(&buff, &buff_size, f) == -1) {
        printf("empty_file\n");
        exit(0);
    }

    size_t v_count = atoi(strtok(buff, " "));
    size_t e_count = atoi(strtok(0, " "));

    graph_t* g = create_graph(v_count, e_count);
    
    size_t i = 0; size_t j = 0; int w = 0; 
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

int shortest_shortest_path(int** distances, graph_t* g) {
    int ssp = 0; 
    for (size_t i = 0; i < g->v_count; ++i) {
        for (size_t j = 0; j < g->v_count; ++j) {
            if (distances[i][j] < ssp) ssp = distances[i][j];
        }
    }
    return ssp;
}