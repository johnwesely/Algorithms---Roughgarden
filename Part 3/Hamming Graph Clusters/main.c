#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hash-table.h"
#include "union-find.h"

size_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream);
hash_table_t* read_hash_table(FILE* f);
union_find_t* read_union_find(FILE* f);
size_t* read_node_list(FILE* f);
size_t read_bit_len(FILE* f);
size_t bit_string_to_unsigned(char* s);
size_t find_max_clusters(size_t* node_arr, hash_table_t* ht, union_find_t* uf, size_t n_bit);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Hamming Clusters takes 1 command line argument");
        return EXIT_FAILURE;
    }

    // fix this memory error;
    size_t buff_size = 256;
    char* path_buff = malloc(buff_size);
    getcwd(path_buff, 256);
    char* test = malloc(buff_size);
    strcpy(test, "/test");
    path_buff = strcat(path_buff, test);

    if (chdir(path_buff) == -1) {
        printf("%s\n", path_buff);
        perror("test folder could not be opened ");
        return EXIT_FAILURE;
    }

    free(path_buff);
    free(test);

    FILE* f = fopen(argv[1], "r");
    
    if (!f) {
        perror("could not open file ");
        return EXIT_SUCCESS;
    }

    union_find_t* uf = read_union_find(f);
    hash_table_t* ht = read_hash_table(f);
    size_t* node_arr = read_node_list(f);
    size_t n_bit = read_bit_len(f);
    fclose(f);

    printf("the maximum number of clusters is %zu\n", find_max_clusters(node_arr, ht, uf, n_bit));
    //print_union_find(uf);

    destroy_union_find(uf);
    destroy_hash_table(ht);
    free(node_arr);

    return EXIT_SUCCESS;
}

hash_table_t* read_hash_table(FILE* f) {
    fseek(f, 0, SEEK_SET);

    size_t buff_size = 64;
    char* buff = calloc(buff_size, 1);

    if (!buff) {
        perror("buff allocation failed ");
        exit(EXIT_FAILURE);
    }

    if (getline(&buff, &buff_size, f) == -1) {
        printf("empty file\n");
        exit(EXIT_FAILURE);
    }

    hash_table_t* ht = create_hash_table(300007);
    strtok(buff, " ");
    size_t len = atoi(strtok(0, " "));
    size_t i = 0;

    while (getline(&buff, &buff_size, f) != -1) {
        append_hash(bit_string_to_unsigned(buff), i++, ht);
    }

    free(buff);
    fseek(f, 0, SEEK_SET);
    return ht;
}

size_t* read_node_list(FILE* f) {
    fseek(f, 0, SEEK_SET);

    size_t buff_size = 64;
    char* buff = calloc(buff_size, 1);

    if (!buff) {
        perror("read node list buff allocation failed ");
        exit(EXIT_FAILURE);
    }

    if (getline(&buff, &buff_size, f) == -1) {
        printf("empty file\n");
        exit(EXIT_FAILURE);
    }

    size_t* ret = malloc(sizeof(size_t) * atoi(strtok(buff, " ")));
    
    size_t i = 0;
    while (getline(&buff, &buff_size, f) != -1) {
        ret[i++] = bit_string_to_unsigned(buff);
    }

    free(buff);
    fseek(f, 0, SEEK_SET);
    return ret;
}

union_find_t* read_union_find(FILE* f) {
    fseek(f, 0, SEEK_SET);
    size_t buff_size = 64;
    char* buff = calloc(buff_size, 1);

    if (!buff) {
        perror("buff allocation failed ");
        exit(EXIT_FAILURE);
    }

    if (getline(&buff, &buff_size, f) == -1) {
        printf("empty file\n");
        exit(EXIT_FAILURE);
    }

    union_find_t* ret = create_union_find(atoi(strtok(buff, " ")));

    free(buff);
    fseek(f, 0, SEEK_SET);
    return ret;
}

size_t read_bit_len(FILE* f) {
    fseek(f, 0, SEEK_SET);
    size_t buff_size = 64;
    char* buff = calloc(buff_size, 1);

    if (!buff) {
        perror("buff allocation failed ");
        exit(EXIT_FAILURE);
    }

    if (getline(&buff, &buff_size, f) == -1) {
        printf("empty file\n");
        exit(EXIT_FAILURE);
    }

    strtok(buff, " ");
    size_t ret = atoi(strtok(0, " "));

    free(buff);
    fseek(f, 0, SEEK_SET);
    return ret;
}

size_t bit_string_to_unsigned(char *s) {
    unsigned int ret = 0;
    int i = 0;

    if (s == NULL) {
        printf("empty string passed to bit_string_to_unsigned\n");
        exit(EXIT_FAILURE);
    }

    while (s[i] == '0' || s[i] == '1' || s[i] == ' ') {
        if (s[i] == '0' || s[i] == '1') {
            ret <<= 1;         // shift ret to left
            ret += s[i]-'0';   // add one or zero ret, setting rightmost bit
        }
        ++i;
    }

    return ret;
}

size_t find_max_clusters(size_t* node_arr, hash_table_t* ht, union_find_t* uf, size_t n_bit) {
    size_t diff_by_one, diff_by_two, found;

    for (size_t i = 0; i < uf->size; ++i) {
        for (size_t j = 0; j < n_bit; ++j) {
            // search for nodes with distance of 1 in ht
            diff_by_one = node_arr[i] ^ (1 << j);
            found = hash_search(diff_by_one, ht);
            if (found) {
                if (find(i, uf) != find(found, uf)) { // if node_arr[i] and found have different parents, union clusters
                    make_union(i, found, uf);
                }
            }
            for (size_t k = 0; k < n_bit; ++k) {
                // search for nodes with distance of 2 in ht
                size_t diff_by_two = diff_by_one ^ (1 << k);
                found = hash_search(diff_by_two, ht);
                if (found) {
                    if (find(i, uf) != find(found, uf)) {
                        make_union(i, found, uf);
                    }
                }
            }
        }
    }

    size_t clusters = 0;
    size_t* c_map = calloc(uf->size, sizeof(size_t));

    for (size_t i = 0; i < uf->size; ++i) {
        if (!c_map[find(i, uf)]) {
            ++clusters;
        }
        ++c_map[uf->parent_ids[i]];
    }

    return clusters;
}

