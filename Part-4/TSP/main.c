#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "bit-sets.h"
#include "tsp.h"

typedef struct __coordinate {
    float x;
    float y;
} coordinate_t;

size_t** read_file_to_graph_and_initialize_bit_set(FILE* f, bit_set_t** bit_set);

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

    bit_set_t* bit_set = 0;
    size_t** g = read_file_to_graph_and_initialize_bit_set(f, &bit_set);
    printf("the shortest salesman path is %zu\n", find_shortest_path(g, bit_set));
    destroy_bit_set(bit_set);
    
    return EXIT_SUCCESS;
}

size_t** read_file_to_graph_and_initialize_bit_set(FILE* f, bit_set_t** bit_set) {
    fseek(f, 0, SEEK_SET);
    size_t buff_size = 256;
    char* buff = calloc(buff_size, 1);

    if (getline(&buff, &buff_size, f) == -1) {
        printf("empty file\n");
        exit(EXIT_FAILURE);
    }

    size_t size = atoi(buff);
    *bit_set = create_bit_set(size);
    coordinate_t* coordinates = malloc(sizeof(coordinate_t) * size);
    size_t i = 0;

    while (getline(&buff, &buff_size, f) != -1) {
        coordinates[i].x = atof(strtok(buff, " "));
        coordinates[i++].y = atof(strtok(0, " "));
    }

    size_t** g = malloc(sizeof(size_t*) * size);
    for (i = 0; i < size; ++i) {
        g[i] = malloc(sizeof(size_t) * size);
    }

    for (i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            size_t x = coordinates[i].x;
            size_t y = coordinates[i].y;
            size_t z = coordinates[j].x;
            size_t w = coordinates[j].y;
            g[i][j] = sqrt(pow(x-z, 2) + pow(y-w, 2));
        }
    }

    free(buff);
    free(coordinates);
    return g;
}
