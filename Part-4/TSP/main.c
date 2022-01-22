#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tsp.h"

graph_t read_graph_from_file(FILE* f, size_t* size_pointer);
void destroy_graph(graph_t g, size_t n);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("TSP takes one command line arugments");
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

    size_t n = 0;
    graph_t g = read_graph_from_file(f, &n);
    fclose(f);

    printf("the shortest salesman tour is %lf\n", find_shortest_tour(g, n));

    destroy_graph(g, n);
    return EXIT_SUCCESS;
}

graph_t read_graph_from_file(FILE*f, size_t* size_pointer) {
    fseek(f, 0, SEEK_SET);
    size_t buff_size = 256;
    char* buff = calloc(buff_size, 1);

    if (getline(&buff, &buff_size, f) == -1) {
        printf("empty file\n");
        exit(EXIT_FAILURE);
    }

    size_t size = atoi(buff);
    *size_pointer = size;
    coordinate_t* coordinates = malloc(sizeof(coordinate_t) * size);
    size_t i = 0;

    while (getline(&buff, &buff_size, f) != -1) {
        coordinates[i].x = atof(strtok(buff, " "));
        coordinates[i++].y = atof(strtok(0, " "));
    }

    graph_t g = malloc(sizeof(double*) * size);
    for (i = 0; i < size; ++i) {
        g[i] = malloc(sizeof(double) * size);
    }

    for (i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            double x = coordinates[i].x;
            double y = coordinates[i].y;
            double z = coordinates[j].x;
            double w = coordinates[j].y;
            g[i][j] = sqrt(pow(x-z, 2) + pow(y-w, 2));
        }
    }

    free(buff);
    free(coordinates);
    return g;
}

void destroy_graph(graph_t g, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        free(g[i]);
    }
    free(g);
}
