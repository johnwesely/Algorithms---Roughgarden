#include <stdio.h>
#include <stdlib.h>

typedef struct __node node_t;
typedef struct __bucket bucket_t;

typedef struct __hash_table {
    size_t     size;
    bucket_t*  buckets;
} hash_table_t;

struct __node {
    long int  key;
    node_t*   next;
};

struct __bucket {
    node_t* head;
};

long int hash_64_shift(long int key);
hash_table_t* create_hash_table(size_t size);
void destroy_hash_table(hash_table_t* ht);
size_t hash_search(long int key, hash_table_t* ht);
void append_hash(long int key, hash_table_t* ht);
void append_node(long int val, bucket_t* bucket);
size_t search_bucket(long int key, bucket_t* bucket);
void free_list(node_t* node);
size_t file_arr_len(FILE* f);
long int* file_to_arr(FILE* f);
size_t count_empty_buckets(hash_table_t* t);
size_t count_2_sum(size_t* sum_map, size_t len);
void analyze_hash(hash_table_t* ht);

int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("2-sum takes 1 command line argument");
        return EXIT_FAILURE;
    }

    long int t_zero = atol(argv[2]);
    long int t_n    = atol(argv[3]);
    long int num_in_range = t_n - t_zero + 1;
    printf("num_in_range = %ld\n", num_in_range);

    FILE* f = fopen(argv[1], "r");

    if (f == 0) {
        perror("failed to load argv[1] ");
        return EXIT_FAILURE;
    }

    long int* arr = file_to_arr(f);
    size_t arr_len = file_arr_len(f);
    hash_table_t* ht = create_hash_table(1500007);

    for (size_t i = 0; i < arr_len; ++i) {
        append_hash(arr[i], ht);
    }

    size_t* sum_map = calloc(num_in_range, sizeof(size_t));

    for (size_t i = 0; i < arr_len; ++i) {
        long int target = t_zero - arr[i]; 
        //printf("target for x = %ld is y = %ld, x + y = %ld\n", arr[i], t_zero - arr[i], arr[i] + (t_zero-arr[i]));
        for (long int j = 0; j < num_in_range; ++j) {
            if (sum_map[j]) continue;
            
            if (target == arr[i]) continue; 

            if (hash_search(target + j, ht)) {
                ++sum_map[j];
            }
        }
    }

    printf("the number of 2-sums is %zu\n", count_2_sum(sum_map, num_in_range));
    //analyze_hash(ht);

    free(sum_map);
    destroy_hash_table(ht);
    fclose(f);
    free(arr);

    return EXIT_SUCCESS;
}

long int hash_64_shift(long key) {
    key = (~key) + (key << 21); // key = (key << 21) - key - 1;
    key = key ^ ((unsigned long)key >> 24);
    key = (key + (key << 3)) + (key << 8); // key * 265
    key = key ^ ((unsigned long)key >> 14);
    key = (key + (key << 2)) + (key << 4); // key * 21
    key = key ^ ((unsigned long)key >> 28);
    key = key + (key << 31);
    return key;
}
/*
long int hash_64_shift(long key) {
    return key;
}
*/

hash_table_t* create_hash_table(size_t size) {
    hash_table_t* ret = malloc(sizeof(hash_table_t));

    if (ret == 0) {
        printf("ht malloc failed\n");
        exit(0);
    }

    ret->size = size;
    ret->buckets = calloc(size, sizeof(bucket_t));

    if (ret->buckets == 0) {
        printf("buckets calloc failed");
        exit(0);
    }

    return ret;
}

void destroy_hash_table(hash_table_t* ht) {
    for (size_t i = 0; i < ht->size; ++i) {
        free_list(ht->buckets[i].head);
    }
    free(ht->buckets);
    free(ht);
}

size_t hash_search(long int key, hash_table_t* ht) {
    long int hk = hash_64_shift(key) % ht->size;
    return search_bucket(key, &ht->buckets[hk]);
}

void append_hash(long int key, hash_table_t* ht) {
    long int hk = hash_64_shift(key) % ht->size;
    append_node(key, &ht->buckets[hk]);
}

void append_node(long int val, bucket_t* bucket) {
    node_t* node = malloc(sizeof(node_t));
    node->next = bucket->head;
    node->key = val;
    bucket->head = node;
}

size_t search_bucket(long int key, bucket_t* bucket) {
    node_t* current = bucket->head;
    while (current) {
        if (current->key == key) return 1;
        current = current->next;
    }
    return 0;
}

void free_list(node_t* node) {
    if (!node) return;
    if (node->next) {
        free_list(node->next);
    }
    free(node);
}

long int* file_to_arr(FILE* f) {
    fseek(f, 0, SEEK_SET);
    size_t len = file_arr_len(f);
    long int* ret = malloc(sizeof(long int) * len);

    if (ret == 0) {
        printf("malloc failed in file_to_arr\n");
        exit(0);
    }

    printf("len = %zu\n", len);

    size_t buff_size = 256;
    char* buff = calloc(buff_size, 1);

    size_t i = 0;

    while (getline(&buff, &buff_size, f) != -1) {
        ret[i++] = atol(buff);
    }

    free(buff);
    return ret;
}

size_t file_arr_len(FILE* f) {
    fseek(f, 0, SEEK_SET);
    size_t len = 0;
    size_t buff_size = 256;
    char* buff = calloc(buff_size, 1);

    while(getline(&buff, &buff_size, f) != -1) {
        ++len;
    }

    fseek(f, 0, SEEK_SET);
    free(buff);
    return len;
}

size_t count_empty_buckets(hash_table_t* ht) {
    size_t ret = 0;
    for (size_t i = 0; i < ht->size; i++) {
        if (!ht->buckets[i].head) {
            ++ret;
        }
    }
    return ret;
}

size_t count_2_sum(size_t* sum_map, size_t len) {
    size_t ret = 0;
    for (size_t i = 0; i < len; ++i) {
        if (sum_map[i]) {
            ++ret;
        }
    }
    return ret;
}

void analyze_hash(hash_table_t* ht) {
    size_t empty_buckets = 0;
    size_t full_buckets = 0;
    size_t number_of_entries = 0;

    for (size_t i = 0; i < ht->size; ++i) {
        if (ht->buckets[i].head) {
            ++full_buckets; 
            node_t* current = ht->buckets[i].head; 
            printf("bucket %zu: -> ", i);
            while (current) {
               ++number_of_entries;
               printf("%ld -> ", current->key);
               current = current->next;
            }
            printf("\n");
        } else {
            ++empty_buckets;
        }
    }

    printf("%zu empty buckets\n", empty_buckets);
    printf("%zu full buckets\n", full_buckets);
    printf("%zu number of entries\n", number_of_entries);
}