#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    uint32_t key;
    size_t key_len;
    char* value;
} HashTableEntry;

uint32_t hash(const uint8_t*, size_t, uint32_t);

#endif