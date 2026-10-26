#include <stdlib.h>

#include "header.h"

//As seen in https://en.wikipedia.org/wiki/MurmurHash

static inline uint32_t scramble(uint32_t k) {
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

uint32_t hash(const uint8_t* key, size_t len)
{
	uint32_t h = seed;
    uint32_t k;
    /* Read in groups of 4. */
    for (size_t i = len >> 2; i; i--) {
        // Here is a source of differing results across endiannesses.
        // A swap here has no effects on hash properties though.
        memcpy(&k, key, sizeof(uint32_t));
        key += sizeof(uint32_t);
        h ^= scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }
    /* Read the rest. */
    k = 0;
    for (size_t i = len & 3; i; i--) {
        k <<= 8;
        k |= key[i - 1];
    }
    // A swap is *not* necessary here because the preceding loop already
    // places the low bytes in the low places according to whatever endianness
    // we use. Swaps only apply when the memory is copied in a chunk.
    h ^= scramble(k);
    /* Finalize. */
	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

HashTable* create_hash_table(size_t capacity){
    HashTable *table = malloc(sizeof(HashTable));

    if (table == NULL) return NULL;

    table->entries = calloc(capacity, sizeof(Node));

    if (table->entries == NULL) {
        free(table);
        return NULL;
    }

    table->capacity = capacity;
    table->size = 0;

    return table;
}

Node* create_node(const char* key, const char* value){
    size_t key_len = strlen(key);
    size_t val_len = strlen(value);

    Node *node = malloc(sizeof(Node));
    if (node == NULL) return NULL;

    node->key = malloc((key_len + 1) * sizeof(char));
    node->value = malloc((val_len + 1) * sizeof(char));

    if (node->key == NULL || node->value == NULL) {
        free(node->key);
        free(node->value);
        free(node);
        return NULL;
    }

    strcpy(node->key, key);
    strcpy(node->value, value);

    node->occupied = 1;
    node->next = NULL;

    return node;
}

int find_node_index(HashTable *table, const char* key){
    size_t len = strlen(key);
    uint32_t index = hash((const uint8_t*)key, len) % table->capacity;

    Node *node = table->entries[index];

    while (node != NULL && node->occupied == 1 && strcmp(node->key, key) != 0){
        index = (index + 1) % table->capacity;
        node = table->entries[index];
    }

    return index;
}

Node* lookup(HashTable *table, const char* key){
    int index = find_node_index(table, key);

    Node *node = table->entries[index];

    return node;
}

void insert_hash_table(HashTable *table, const char* key, const char* value){

    // if (table->size >= table->capacity * load_factor){
    //     resize_hash_table(table, table->capacity * 2);
    // }

    int index = find_node_index(table, key);

    if (table->entries[index] != NULL){
        Node *node = table->entries[index];

        while (node != NULL && node->next != NULL){
            node = node->next;
            printf("P: %p\n", node);
        }

        node->next = create_node(key, value);
    }
    else {
        table->entries[index] = create_node(key, value);
        table->size++;
    }
}

// void resize_hash_table(HashTable *table, size_t new_capacity){
//     if (new_capacity < table->size){
//         fprintf(stderr, "Cannot resize to smaller capacity");
//         return;
//     }
        

//     HashTable *new_table = create_hash_table(new_capacity);

//     for (size_t i = 0; i < table->capacity; i++){
//         Node *entry = table->entries[i];
//         if (entry->occupied == 0) continue;

//         while (entry != NULL){

//         }

//     }


// }