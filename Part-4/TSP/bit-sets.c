#include <stdio.h>
#include <stdlib.h>
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

size_t create_bit_set(size_t n) { // create bit sets for all permutations of 25 bits

}