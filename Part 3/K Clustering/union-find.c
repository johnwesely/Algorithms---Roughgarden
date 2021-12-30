#include <stdio.h>
#include <stdlib.h>
#include "union-find.h"

union_find_t* create_union_find(size_t size) {
    union_find_t* ret = malloc(sizeof(union_find_t));
    ret->size = size;
    ret->n_sets = size;

    ret->parent_ids = malloc(sizeof(size_t) * size);
    for (size_t i = 0; i < size; ++i) {
        ret->parent_ids[i] = i;
    }

    ret->sets = malloc(sizeof(uf_set_t) * size);
    for (size_t i = 0; i < size; ++i) {
        ret->sets[i].id = i;
        ret->sets[i].size = 1;
        ret->sets[i].children = malloc(sizeof(child_list_t));
        ret->sets[i].children->id = i;
        ret->sets[i].children->next = 0;
    }

    return ret;
}

void free_child_list(child_list_t* current) {
    if (!current) return;

    if (current->next) {
        free_child_list(current->next);
    }
    free(current);
}

void destroy_union_find(union_find_t* uf) {
    free(uf->parent_ids);

    for (size_t i = 0; i < uf->size; ++i) {
        free_child_list(uf->sets[i].children);
    }

    free(uf->sets);
    free(uf);
}

// find parent of member of union find
size_t find(size_t id, union_find_t* uf) {
    return uf->parent_ids[id];
}

// append child list src to end of child list dest
void append_child(child_list_t* src, child_list_t* dest) {
    child_list_t* current = dest;
    
    while (current->next) {
        current = current->next;
    }

    current->next = src;
}

// set children of cl to new_parent
void set_children(size_t new_parent, child_list_t* cl, union_find_t* uf) {
    child_list_t* current = cl;
    while (current) {
        uf->parent_ids[current->id] = new_parent;
        current = current->next;
    }
}

void make_union(size_t set_a, size_t set_b, union_find_t* uf) {
    set_a = find(set_a, uf);      // find parents of set_a and set_b
    set_b = find(set_b, uf);
    if (set_a == set_b) return;   // do nothing if they are the same set

    if (uf->sets[set_a].size > uf->sets[set_b].size) {
        set_children(uf->sets[set_a].id, uf->sets[set_b].children, uf);    // set children of set b to new parent a
        append_child(uf->sets[set_b].children, uf->sets[set_a].children);  // append child list of b to a
        uf->sets[set_a].size += uf->sets[set_b].size;                      // add size of b to a
        uf->sets[set_b].size = 0;                                          // set size of b to 0
        uf->sets[set_b].children = 0;
    } else {
        set_children(uf->sets[set_b].id, uf->sets[set_a].children, uf);    // same but revers
        append_child(uf->sets[set_a].children, uf->sets[set_b].children);
        uf->sets[set_b].size += uf->sets[set_a].size;
        uf->sets[set_a].size = 0;
        uf->sets[set_a].children = 0;
    }
}

void print_union_find(union_find_t* uf) {
    printf("printing union find\n");
    for (size_t i = 0; i < uf->size; ++i) {
        printf("child %zu = parent %zu\n", i, find(i, uf));
    }
}