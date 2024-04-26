//
// Created by Jesko Förster on 18.04.2024.
//
#include <stdbool.h>

#ifndef MAP_H
#define MAP_H
#define MAP_SIZE 100
#define KEY_SIZE 128
#define VALUE_SIZE 128

struct KeyValuePair {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
};

typedef struct {
    struct KeyValuePair table[MAP_SIZE];
} Map;

/**
 * @param key The key to hash.
 * @return The hash value (index).
 */
unsigned int hash(const char* key);

void map_insert_element(Map* map, const char* key, const char* value);

/**
 * @return The value associated with the key, or NULL if not found.
 */
const char* map_get_element(Map* map, const char* key);

void map_delete_element(Map* map, const char* key);

#endif
