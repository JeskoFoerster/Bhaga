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

/**
 * @struct Map
 * @brief Structure representing a hash map.
 */
struct Map {
    struct KeyValuePair* table[MAP_SIZE];
};

/**
 * @brief Hash function to convert a string key into an index.
 * @param key The key to hash.
 * @return The hash value (index).
 */
unsigned int hash(const char* key);

/**
 * @brief Function to create a new key-value pair.
 * @param key The key.
 * @param value The value.
 * @return Pointer to the created key-value pair.
 */
struct KeyValuePair* create_key_value_pair(const char* key, const char* value);

/**
 * @brief Creates a new empty map.
 * @return Pointer to the newly created map.
 */
struct Map* map_create();

/**
 * @brief Function to map_insert a key-value pair into the map.
 * @param map Pointer to the map.
 * @param key The key.
 * @param value The value.
 */
void map_insert(struct Map* map, const char* key, const char* value);

/**
 * @brief Function to retrieve the value associated with a key.
 * @param map Pointer to the map.
 * @param key The key.
 * @return The value associated with the key, or -1 if not found.
 */
const char* map_get(struct Map* map, const char* key);

/**
 * @brief Function to map_delete a key-value pair from the map.
 * @param map Pointer to the map.
 * @param key The key.
 */
void map_delete(struct Map* map, const char* key);

/**
 * @brief Function to free the memory allocated for the map.
 * @param map Pointer to the map.
 */
void map_destroy(struct Map* map);

#endif
