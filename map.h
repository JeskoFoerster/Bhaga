//
// Created by Jesko Förster on 18.04.2024.
//
#include <stdbool.h>

#ifndef MAP_H
#define MAP_H
#define MAP_SIZE 100

struct KeyValuePair {
    const char* key;
    const char* value;
    struct KeyValuePair* nextPair;
};


struct Map {
    struct KeyValuePair* table[MAP_SIZE];
};

/**
 * @param key The key to hash.
 * @return The hash value (index).
 */
unsigned int hash(const char* key);

struct KeyValuePair* create_key_value_pair(const char* key, const char* value);

struct Map* map_create();

void map_insert(struct Map* map, const char* key, const char* value);

/**
 * @return The value associated with the key, or NULL if not found.
 */
const char* map_get(struct Map* map, const char* key);

void map_delete(struct Map* map, const char* key);

void map_destroy(struct Map* map);

#endif
