//
// Created by Jesko Förster on 18.04.2024.
//

#include <sys/shm.h>
#include <stdlib.h>
#include "map.h"

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
char* map_getall_elements(Map* map) {
    char* results = malloc((MAP_SIZE+1) * sizeof(char));
    char ending[4];
    for (int i = 0; i < MAP_SIZE; i++) {
        if (map->table[i].key != NULL && map->table[i].value != NULL) {
            strcat(results, map->table[i].key);
            strcat(results, " ");
            strcat(results, map->table[i].value);
            sprintf(ending,"\n\r");
            //strcat(results, ending);
        } else {
            strcat(results, "NULL!");
        }
    }
    return results;
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

Map * createSharedMemoryMap(){
    // Erstellen des Shared Memory
    int shmid = shmget(IPC_PRIVATE, sizeof(Map), IPC_CREAT|0600);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Anhängen des Shared Memory an den Prozess
    Map * shar_mem_map = (Map *)shmat(shmid, 0, 0);
    if (shar_mem_map == (void *)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    return shar_mem_map;
}