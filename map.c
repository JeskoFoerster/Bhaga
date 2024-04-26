//
// Created by Jesko Förster on 18.04.2024.
//

#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void map_insert_element(Map* map, const char* key, const char* value) {
    // Check if the key already exists and update the value if found
    for (int i = 0; i < MAP_SIZE; i++) {
        if (strcmp(map->table[i].key, key) == 0) {
            // Key already exists, update the value
            int valueSize = sizeof(map->table[i].value) - 1;
            strncpy(map->table[i].value, value, valueSize);
            map->table[i].value[valueSize] = '\0';
            return;
        }
    }

    // Key not found, search for an empty slot
    for (int i = 0; i < MAP_SIZE; i++) {
        if (map->table[i].key[0] == '\0') {
            // Found an empty slot, insert the new entry
            int valueSize = sizeof(map->table[i].value) - 1;
            int keySize = sizeof(map->table[i].key) - 1;

            strncpy(map->table[i].key, key, keySize);
            map->table[i].key[keySize] = '\0';

            strncpy(map->table[i].value, value, valueSize);
            map->table[i].value[valueSize] = '\0';
            return;
        }
    }

    // Array is fully occupied
    printf("The storage is full, no further values can be saved.\n");
}

const char* map_get_element(Map* map, const char* key) {
    // Check if the key exists
    for (int i = 0; i < MAP_SIZE; i++) {
        if (strcmp(map->table[i].key, key) == 0) {
            // Key found, return the value
            return map->table[i].value;
        }
    }
    // Key not found
    return NULL;
}

void map_delete_element(Map* map, const char* key) {
    // Check if the key exists
    for (int i = 0; i < MAP_SIZE; i++) {
        if (strcmp(map->table[i].key, key) == 0) {
            // Key found, delete the entry
            map->table[i].key[0] = '\0';
            map->table[i].value[0] = '\0';
            return;
        }
    }
}