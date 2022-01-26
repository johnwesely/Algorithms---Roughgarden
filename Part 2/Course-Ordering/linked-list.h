#include <stdlib.h>
#include <stdio.h>

#ifndef LL_H
#define LL_H

// node of linked list
typedef struct __node node_t;
struct __node {
    size_t  value;
    node_t* next;
};

// linked list
typedef struct __linked_list {
    node_t* head;
} linked_list_t;

// subroutines for creating and destroying linked lists
void append_list(linked_list_t* linked_list, size_t value);
void destroy_nodes(node_t* node);
void destroy_list(linked_list_t* linked_list);

#endif