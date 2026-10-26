#ifndef HASH_TABLE_HEADER_H
#define HASH_TABLE_HEADER_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SET_SEED(x) (seed = x)

static uint32_t seed;
static float load_factor;


typedef struct Node {
    char* key;
    char* value;
    int occupied;
    struct Node *next;
} Node;

typedef struct {
    Node **entries;
    size_t capacity;
    size_t size;
} HashTable;

uint32_t hash(const uint8_t*, size_t);
HashTable* create_hash_table(size_t);
Node* lookup(HashTable*, const char*);
void insert_hash_table(HashTable*, const char*, const char*);


#endif