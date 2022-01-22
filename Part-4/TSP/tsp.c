#include "tsp.h"
#include <limits.h>

size_t is_even(size_t n) {
    if ((n ^ 1) == (n + 1)) {
        return 1;
    } else {
        return 0; 
    }
}

double find_shortest_path(double** g, bit_set_t* bit_set) {
    bit_set->bsm_arr[1].value = 0;
    bit_set->bsm_arr[1].last_j = 0;

    // determine the shortest path visiting each vertex once for each possible set up to n-1 vertices
    for (size_t i = 2; i < bit_set->size; ++i) {
        printf("outer loop %zu\n", i);
        node_t* current_gen = bit_set->bit_group_arr[i].head;  // list of sets with current generations number of members

        while (current_gen) {
            // if current_gen set has even numerical representation, set doesn't contain first stop, skip to next set
            /*
            if (is_even(current_gen->index)) {  
                current_gen = current_gen->next;
                continue;
            }
            */

            bsm_t* current_member = &bit_set->bsm_arr[current_gen->index];  // current set
            node_t* previous_gen = bit_set->bit_group_arr[i-1].head;       // previous generation of sets
            /* 
            while (previous_gen) {
                printf("shortest path for set %zu ending at vertice %zu: %lf\n", previous_gen->index, bit_set->bsm_arr[previous_gen->index].last_j, bit_set->bsm_arr[previous_gen->index].value);
                previous_gen = previous_gen->next;
            }
            
            previous_gen = bit_set->bit_group_arr[i-1].head;
*/
            while (previous_gen) {
                //printf("previous gen index is %zu\n", previous_gen->index);
                bsm_t* previous_member = &bit_set->bsm_arr[previous_gen->index];

                // if previous gen is even, set doesn't contain first stop, or previous set being checked has more than one different bit than current set
                if (/*is_even(previous_member->key) || */count_bits(current_member->key ^ previous_member->key) > 1) {
                    previous_gen = previous_gen->next;  // skip to next set in set of previous sets
                    continue;
                }

                size_t next_stop_index = set_bit_index(current_member->key ^ previous_member->key);
                //printf("previous member is %zu, previous member.last_j is %zu, next stop index is %zu\n", previous_member->key, previous_member->last_j, next_stop_index);
                if (previous_member->value + g[previous_member->last_j][next_stop_index] == current_member->value) {
                    printf("two paths with same value for set\n");
                }
                if (previous_member->value + g[previous_member->last_j][next_stop_index] < current_member->value) {
                    //printf("updating set %zu with value %zu and last_j %zu\n", current_member->key, previous_member->value + g[previous_member->last_j][next_stop_index], next_stop_index);
                    current_member->value = previous_member->value + g[previous_member->last_j][next_stop_index];
                    current_member->last_j = next_stop_index;
                }

                previous_gen = previous_gen->next;
            }

            current_gen = current_gen->next;
        }
    }

    // pick minimum of all set n-1s + distance to final vertix + distance to first vertex
    double ret = 9000000;
    size_t final_gen = bit_set->bit_group_arr[bit_set->size].head->index;
    node_t* previous_gen = bit_set->bit_group_arr[bit_set->size-1].head;

    while (previous_gen) {
        if (is_even(previous_gen->index)) {
            previous_gen = previous_gen->next;
            continue;
        }

        bsm_t* current_member = &bit_set->bsm_arr[previous_gen->index];
        size_t next_stop_index = set_bit_index(current_member->key ^ final_gen);
        double candidate = current_member->value + g[current_member->last_j][next_stop_index] + g[next_stop_index][0];
        printf("potential candidate %lf\n", candidate);
        if (ret > candidate) {
            ret = candidate;
        }
        
        previous_gen = previous_gen->next;
    }

    return ret;
}