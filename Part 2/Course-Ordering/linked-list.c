#include "linked-list.h"

// subroutines for creating and destroying linked lists
void append_list(linked_list_t* linked_list, size_t value) {
    node_t* node = malloc(sizeof(node_t));
    node->value = value;
    node->next = linked_list->head;
    linked_list->head = node;
}

void destroy_nodes(node_t* node) {
    if (!node) return;

    destroy_nodes(node->next);
    free(node);
}

void destroy_list(linked_list_t* linked_list) {
    destroy_nodes(linked_list->head);
    free(linked_list);
}