#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t seed;

typedef struct {
    char* key;
    size_t key_len;
    char* value;
} HashTableEntry;

uint32_t hash(const uint8_t*, size_t);

#endif