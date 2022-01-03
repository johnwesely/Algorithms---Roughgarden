#include <stdlib.h>
#include <stdio.h>
#include "hash-table.h"

void append_node(size_t val, size_t id, bucket_t* bucket);
size_t search_bucket(size_t key, bucket_t* bucket);
void free_list(node_t* node);
void analyze_hash(hash_table_t* ht);

size_t hash(size_t a) {
   a = (a+0x7ed55d16) + (a<<12);
   a = (a^0xc761c23c) ^ (a>>19);
   a = (a+0x165667b1) + (a<<5);
   a = (a+0xd3a2646c) ^ (a<<9);
   a = (a+0xfd7046c5) + (a<<3);
   a = (a^0xb55a4f09) ^ (a>>16);
   return a;
}

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

size_t hash_search(size_t key, hash_table_t* ht) {
    size_t hk = hash(key) % ht->size;
    return search_bucket(key, &ht->buckets[hk]);
}

// appends key id pair to hash table
void append_hash(size_t key, size_t id, hash_table_t* ht) {
    size_t hk = hash(key) % ht->size;
    append_node(key, id, &ht->buckets[hk]);
}

// appends key id pair to bucket in hash table
void append_node(size_t val, size_t id, bucket_t* bucket) {
    node_t* node = malloc(sizeof(node_t));
    node->next = bucket->head;
    node->key = val;
    node->id = id;
    bucket->head = node;
}

// helper function for has search, searches linked list in bucket
size_t search_bucket(size_t key, bucket_t* bucket) {
    node_t* current = bucket->head;
    while (current) {
        if (current->key == key) return current->id;
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