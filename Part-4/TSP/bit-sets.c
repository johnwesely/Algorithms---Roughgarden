#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "bit-sets.h"

size_t set_bit_index(size_t n) {  // returns index of first set bit
    for (size_t i = 0; i < 32; ++i) {
        if (n % 2) return i;
        >>n;
    }

    return 0;
}

size_t count_bits(size_t n) {    // returns number of set bits
    size_t ret = 0;

    for (size_t i = 0; i < 32; ++i) {
        if (n % 2) ++ret;
        >>n;
    }

    return ret;
}

void append_bit_set_member(size_t n, linked_list_t* bga) {  // append index of bit set member to appropriate list based on number of set bits
    node_t* node = malloc(sizeof(node_t));
    size_t bga_index = count_bits(n);
    node->index = n;
    node->next = bga[bga_index].head;
    bga[bga_index].head = node;
}

size_t create_bit_set(size_t n) { // create bit sets for all permutations of n bits, for sets with up to 32 members
    size_t set_max = pow(2, n);
    bit_set_t* bit_set = malloc(sizeof(bit_set_t));
    bit_set->bit_group_arr = calloc(n + 1, sizeof(linked_list_t));
    bit_set->bsm_arr = malloc(sizeof(bsm_t) * set_max);
    bit_set->size = n;
    
    for (size_t i = 0; i < set_max; ++i) {
        bit_set->bsm_arr[i].key = i;
        bit_set->bsm_arr[i].value = UINT_MAX;
        bit_set->bsm_arr[i].last_j = UINT_MAX;
        append_bit_set_member(i, bit_set->bit_group_arr);
    }

    return bit_set;
}

void destroy_linked_list(node_t* node) {
    if (!node) return;

    destroy_linked_list(node->next);
    free(node);
}

void destroy_bit_set(bit_set_t* bit_set) {
    free(bit_set->bsm_arr);

    for (size_t i = 0; i < bit_set->size + 1; ++i) {
        destroy_linked_list(bit_set->bit_group_arr[i].head);
    }

    free(bit_set->bit_group_arr);
    free(bit_set);
}