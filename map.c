//
// Created by Jesko Förster on 18.04.2024.
//

#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int hash(const char* key) {
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        //link: https://www.geeksforgeeks.org/why-does-javas-hashcode-in-string-use-31-as-a-multiplier/
        hash = 31 * hash + key[i];
    }
    return hash % MAP_SIZE;
}


struct KeyValuePair* create_key_value_pair(const char* key, const char* value) {
    struct KeyValuePair* pair = (struct KeyValuePair*)malloc(sizeof(struct KeyValuePair));
    if (pair == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    pair->key = strdup(key);
    pair->value = strdup(value); // Duplicate the value string
    pair->nextPair = NULL;
    return pair;
}


struct Map* map_create() {
    struct Map* map = (struct Map*)malloc(sizeof(struct Map));
    if (map == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // Initialize all table entries to NULL
    for (int i = 0; i < MAP_SIZE; i++) {
        map->table[i] = NULL;
    }

    return map;
}


void map_insert_element(struct Map* map, const char* key, const char* value) {
    //catch nullptr here because in the map NULL means not set
    if(key == NULL){
        printf("Key is a nullptr! Unable to create entry.\n");
        exit(1);
    }

    unsigned int index = hash(key);
    struct KeyValuePair* pair = create_key_value_pair(key, value);

    // Handle collision by chaining
    bool collision = map->table[index] != NULL;
    if (collision) {
        // Collision detected, add to the end of the chain
        struct KeyValuePair* current = map->table[index];
        while (current->nextPair != NULL) {
            current = current->nextPair;
        }
        current->nextPair = pair;
    } else {
        map->table[index] = pair;
    }
}


const char* map_get_element(struct Map* map, const char* key) {
    //catch nullptr here because in the map NULL means not set
    if(key == NULL){
        printf("Key is a nullptr! Unable to create entry.\n");
        exit(1);
    }

    unsigned int index = hash(key);
    struct KeyValuePair* current = map->table[index];

    // Search through the chain for the key
    while (current != NULL) {
        //compares the key of the map with the key in the list
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->nextPair;
    }

    // Key not found
    return NULL;
}


void map_delete_element(struct Map* map, const char* key) {
    //catch nullptr here because in the map NULL means not set
    if(key == NULL){
        printf("Key is a nullptr! Unable to create entry.\n");
        exit(1);
    }

    unsigned int index = hash(key);
    struct KeyValuePair* current = map->table[index];
    struct KeyValuePair* previous = NULL;

    // Search through the chain for the key
    while (current != NULL) {
        //compares the key of the map with the key in the list
        if (strcmp(current->key, key) == 0) {
            // Key found, remove the pair from the chain
            if (previous == NULL) {
                // If the key-value pair is the head of the chain
                map->table[index] = current->nextPair;
            } else {
                // If the key-value pair is not the head of the chain
                previous->nextPair = current->nextPair;
            }
            // Free memory allocated for the key and the pair itself
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        // Move to the nextPair pair in the chain
        previous = current;
        current = current->nextPair;
    }
}


void map_destroy(struct Map* map) {
    for (int i = 0; i < MAP_SIZE; i++) {
        struct KeyValuePair* current = map->table[i];
        bool chainEnd = current == NULL;
        while (!chainEnd) {
            struct KeyValuePair* temp = current;
            current = current->nextPair;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(map);
}
