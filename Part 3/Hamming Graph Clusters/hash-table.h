#include <stdlib.h>
#include <stdlib.h>

#ifndef HT_H
#define HT_H

typedef struct __node node_t;
typedef struct __bucket bucket_t;

typedef struct __hash_table {
    size_t     size;
    bucket_t*  buckets;
} hash_table_t;

struct __node {
    size_t    key;
    size_t    id;
    node_t*   next;
};

struct __bucket {
    node_t* head;
};

// Hash Table API
hash_table_t* create_hash_table(size_t size);
void destroy_hash_table(hash_table_t* ht);
size_t hash_search(size_t key, hash_table_t* ht);   // returns id of key in ht, or zero if not found
void append_hash(size_t key, size_t id, hash_table_t* ht);
void analyze_hash(hash_table_t* ht);

#endif