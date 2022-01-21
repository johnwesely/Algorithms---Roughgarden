// library for bit set operations
#include <stdlib.h>
#include <stdio.h>

#ifndef BS_H
#define BS_H

typedef struct __bit_set_member {
    size_t key;       // key for bit set member, also bit representation of all vertices contained in set
    size_t value;     // value of shortest path containing all vertices in bit representation of key
    size_t last_j;    // id of last visted vertex 
} bsm_t;

typedef struct __linked_list_node {
    size_t  index;      // index of bit set member
    node_t* next;       // pointer to next member in list
} node_t;

typedef struct __linked_list_head {
    node_t* head;
} linked_list_t;

typedef struct __bit_set {
    size_t          size;            // size of bit_set
    linked_list_t*  bit_group_arr;   // array of bit set member lists, index is number of on bits in numberical representation 
    bsm_t*          bsm_arr;         // array of bit set members, index is key and numerical representation of bit set
} bit_set_t;

size_t set_bit_index(size_t n);  // returns index of first set bit
size_t count_bits(size_t n);     // returns number of set bits
size_t create_bit_set(size_t n); // create bit sets for all permutations of 25 bits
void destroy_bit_set(bit_set_t* bit_set); // free bit set

#endif