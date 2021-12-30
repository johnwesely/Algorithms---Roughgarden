#include <stdio.h>
#include <stdlib.h>

#ifndef UF_H
#define UF_H

// linked list of children in a set
typedef struct __child_list child_list_t;
struct __child_list {
    size_t        id;
    child_list_t* next;
};

// set in union find
typedef struct __set {
    size_t        id;         // set id
    size_t        size;       // set size
    child_list_t* children;   // list containing ids of all children in set
} uf_set_t;

typedef struct __union_find {
    size_t    size;           // size of union find
    size_t    n_sets;         // number of sets in UF
    size_t*   parent_ids;     // map representing parent ids of every member of UF
    uf_set_t* sets;           // array of sets
} union_find_t;

// Union Find API
union_find_t* create_union_find(size_t size);
void destroy_union_find(union_find_t* uf);
size_t find(size_t id, union_find_t* uf);
void make_union(size_t set_a, size_t set_b, union_find_t* uf);
void print_union_find(union_find_t* uf);

#endif