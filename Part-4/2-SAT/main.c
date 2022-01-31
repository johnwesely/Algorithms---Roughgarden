#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "unweighted-directed-graph.h"
#include "Korasaju.h"

graph_t* read_file_to_graph(FILE* file, size_t direction, size_t* vc_pointer);
size_t two_satisfiability(size_t* scc_parent_map, size_t variable_count);
int getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream);
void print_scc_parent_map(size_t* scc_parent_map, size_t variable_count);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("2-SAT takes one command line arugments");
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

    size_t variable_count = 0;
    graph_t* forward_graph = read_file_to_graph(f, FORWARD, &variable_count);
    graph_t* reverse_graph = read_file_to_graph(f, REVERSE, &variable_count);

    size_t* scc_parent_map = find_strongly_connected_components(reverse_graph, forward_graph);
    printf("2-SAT %s\n", two_satisfiability(scc_parent_map, variable_count) ? "is satsifiable" : "is not satisfiable");
    free(scc_parent_map);

    return EXIT_SUCCESS;
}

size_t two_satisfiability(size_t* scc_parent_map, size_t variable_count) {
    for (size_t i = 0, j = variable_count * 2 - 1; i < variable_count; ++i, --j) {
        if (scc_parent_map[i] == scc_parent_map[j]) {   // if x and not x are in same SCC
            return 0;                                   // return false
        }
    }

    return 1;   // if no unsatisfiable variables are found return true
}

size_t variable_id_to_index(int variable_id, size_t variable_count) {
    if (variable_id < 0) {
        return variable_id + variable_count;
    } else {
        return variable_id + variable_count - 1;
    }
}

int index_to_variable_id(size_t index, size_t variable_count) {
    if (index >= variable_count) {
        return index + 1 - variable_count;
    } else {
        return index - variable_count;
    }
}

int not(int variable_id) {
    if (variable_id < 0) {
        return abs(variable_id);
    } else {
        return variable_id * -1;
    }
}

graph_t* read_file_to_graph(FILE* file, size_t direction, size_t* vc) {
    fseek(file, 0, SEEK_SET);

    size_t buff_size = 256;
    char* buff = calloc(buff_size, 1);

    if (getline(&buff, &buff_size, file) == -1) {
        printf("empty_file\n");
        exit(EXIT_FAILURE);
    }

    size_t variable_count = atoi(buff);
    *vc = variable_count;
    graph_t* graph = create_graph((variable_count * 2));
    int variable_x, variable_y; 
    size_t implication_a_i, implication_a_j;
    size_t implication_b_i, implication_b_j;

    while (getline(&buff, &buff_size, file) != -1) {
        variable_x = atoi(strtok(buff, " "));
        variable_y = atoi(strtok(0, " "));

        // generate two implications from each clause
        // (x ^ y) == ~x => y and ~y => x
        implication_a_i = variable_id_to_index(not(variable_x), variable_count);
        implication_a_j = variable_id_to_index(variable_y, variable_count);
        implication_b_i = variable_id_to_index(not(variable_y), variable_count);
        implication_b_j = variable_id_to_index(variable_x, variable_count);
        
        // append ("*_i", "*_j") or ("*_j", "*_i") depending on direction of graph
        (direction) ? append_edge(graph, implication_a_i, implication_a_j) : append_edge(graph, implication_a_j, implication_a_i); 
        (direction) ? append_edge(graph, implication_b_i, implication_b_j) : append_edge(graph, implication_b_j, implication_b_i);
    }

    fseek(file, 0, SEEK_SET);
    free(buff);
    return graph;
}

void print_scc_parent_map(size_t* scc_parent_map, size_t variable_count) {
    for (size_t i = 0; i < variable_count * 2; ++i) {
        printf("variable %d parent = %d\n", index_to_variable_id(i, variable_count), index_to_variable_id(scc_parent_map[i], variable_count));
    }
}
