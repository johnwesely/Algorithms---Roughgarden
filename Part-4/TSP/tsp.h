#include <stdio.h>
#include <stdlib.h>
#include "bit-sets.h"

#ifndef TSP_H
#define TSP_H

size_t find_shortest_path(size_t** g, bit_set_t* bit_set);

#endif