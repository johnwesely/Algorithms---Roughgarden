#include <stdio.h>
#include <stdlib.h>
#include "union-find.h"

int main(void) {
    union_find_t* uf = create_union_find(15);
    make_union(0, 1, uf);
    print_union_find(uf);
    make_union(find(0, uf), 2, uf);
    make_union(3, 4, uf);
    print_union_find(uf);
    make_union(find(0, uf), find(3, uf), uf);
    print_union_find(uf);
    make_union(3, 4, uf);
    destroy_union_find(uf);
    return EXIT_SUCCESS;
}