/* prints valid course ordering of curriculum passed in file argv[1]
// curriculum is formatted in format [[v1, j1], [v2, j2], ...]
// where j1 is a prerequisite for v1, v2 is a prerequisite for j2 etc.
// sample input: "[[1,0],[2,0],[3,1],[3,2]]"
// sample output "0, 2, 1, 3"
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linked-list.h"
#include "weighted-undirected-graph.h"


size_t* course_sort(graph_t* g);
graph_t* read_file_to_graph(FILE* f);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Course Schedule 2 takes 1 command line argument\n");
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

    FILE* file = fopen(argv[1], "r");

    if (!file) {
        perror("failure to load file ");
        return EXIT_FAILURE;
    }

    free(test);
    free(path_buff);

    graph_t* curriculum_graph = read_file_to_graph(file);
    fclose(file);
    size_t* possible_course_order = course_sort(curriculum_graph);

    for (size_t i = 0; i < curriculum_graph->vertex_count; ++i) {
        printf("%zu, ", possible_course_order[i]);
    }

    printf("\n");
    free(possible_course_order);
    destroy_graph(curriculum_graph);
    return EXIT_SUCCESS;
}



// returns true if candidate has all prequisites met
size_t satisfied_pre_reqs(graph_t* curriculum_graph, size_t candidate, size_t* visited) {
    node_t* pre_req = curriculum_graph->vertices[candidate].pre_reqs.head;

    // iterate through all re reqs
    while (pre_req) {
         if (!visited[pre_req->value]) return 0;  // if unmet pre-req is found return false
         pre_req = pre_req->next;
    }

    return 1;
}

// breadth first search of graph
void bfs(graph_t* curriculum_graph, linked_list_t* course_list,
        size_t* visited, size_t* order, size_t* i) {

    if (!course_list->head) {         // base case
        destroy_list(course_list);    // bfs is passed an empty course_list
        return;
    }

    linked_list_t* next_list = calloc(1, sizeof(linked_list_t));
    node_t* current_course = course_list->head;

    while (current_course) {
        order[*i] = current_course->value;  // add course to list
        ++*i;
        node_t* candidate_course = curriculum_graph->vertices[current_course->value].edges.head;

        while (candidate_course) {                              // iterate through all edges of current course 
            // if current course has not been added yet and has all pre reqs satisted
            if (!visited[candidate_course->value] && satisfied_pre_reqs(curriculum_graph, candidate_course->value, visited)) {
                append_list(next_list, candidate_course->value);    // add to list for next iteration of bfs
                ++visited[candidate_course->value];                 // mark course as visited
            }
            candidate_course = candidate_course->next;
        }

        current_course = current_course->next;
    }

    destroy_list(course_list);                            // free this iterations course_list

    bfs(curriculum_graph, next_list, visited, order, i);  // call bfs with next iterations course list
}

// returns a list of courses in curriculum graph with no prequisites 
linked_list_t* find_starting_courses(graph_t* curriculum_graph) {
    linked_list_t* ret = calloc(1, sizeof(linked_list_t));

    for (size_t i = 0; i < curriculum_graph->vertex_count; ++i) {
        if (!curriculum_graph->vertices[i].pre_reqs.head) {
            append_list(ret, i);
        }
    }

    return ret;
}

// sorts courses in curriculum graph into a valid order using breadth first search
size_t* course_sort(graph_t* curriculum_graph) {
    size_t* order = malloc(sizeof(size_t) * curriculum_graph->vertex_count);              // return array of sorted courses
    size_t* visited = calloc(curriculum_graph->vertex_count, sizeof(size_t));             // boolean array for whether course has been visited
    size_t i = 0;
    
    // preprocessing step adding all courses without prerequisites to order array 
    linked_list_t* starting_courses = find_starting_courses(curriculum_graph);   // a list of all courses without pre reqs
    linked_list_t* next_courses = calloc(1, sizeof(linked_list_t));              // a list to pass to bfs of courses to check next
    node_t* course = starting_courses->head;

    while (course) {             // iterate through all courses without pre reqs
        ++visited[course->value];
        order[i] = course->value;
        ++i;

        node_t* next_course = curriculum_graph->vertices[course->value].edges.head;                    // add all valid courses to list for bfs
        while (next_course) {
            if (!visited[next_course->value] && satisfied_pre_reqs(curriculum_graph, next_course->value, visited)) {
                append_list(next_courses, next_course->value);
                ++visited[next_course->value];
            }
            next_course = next_course->next;
        }

        course = course->next;
    }
    
    destroy_list(starting_courses);
    bfs(curriculum_graph, next_courses, visited, order, &i);    // call breadth first serach recursively until graph is processed
    free(visited);

    return order;
}

size_t count_vertices_from_file(FILE* file) {
    fseek(file, 0, SEEK_SET); 
    size_t max_vertex_index = 0;
    size_t buff_size = 512;
    char* buff = calloc(buff_size, 1);

    if (getline(&buff, &buff_size, file) == -1) {
        printf("empty file\n");
        exit(EXIT_FAILURE);
    }

    char* n = strtok(buff, "[, ]");
    if (atoi(n) > max_vertex_index) max_vertex_index = atoi(n);
    
    while ((n = strtok(0, "[, ]"))) {
        if (atoi(n) > max_vertex_index) max_vertex_index = atoi(n);
    }

    free(buff);
    fseek(file, 0, SEEK_SET);
    return max_vertex_index + 1;
}

// format of input, list of edges, [[v, j], [v, j], ...]
// [[1,0],[2,0],[3,1],[3,2]]
graph_t* read_file_to_graph(FILE* file) {
    fseek(file, 0, SEEK_SET);
    size_t vertex_count  = count_vertices_from_file(file);
    size_t buff_size = 512;
    char* buff = calloc(buff_size, 1);

    if (getline(&buff, &buff_size, file) == -1) {
        printf("empty file\n");
        exit(EXIT_FAILURE);
    }

    graph_t* curriculum_graph = malloc(sizeof(graph_t));
    curriculum_graph->vertex_count = vertex_count;
    curriculum_graph->vertices = calloc(vertex_count, sizeof(vertex_t));

    for (size_t i = 0; i < vertex_count; ++i) {
        curriculum_graph->vertices[i].id = i;
    }

    char* j_id = strtok(buff, "[, ]");
    char* v_id = strtok(0, "[, ]");

    append_edge(curriculum_graph, atoi(v_id), atoi(j_id));

    while ((j_id = strtok(0, "[, ]\n"))) {
        v_id = strtok(0, "[, ]\n");
        append_edge(curriculum_graph, atoi(v_id), atoi(j_id));
    }

    free(buff);
    fseek(file, 0, SEEK_SET);
    return curriculum_graph;
}
