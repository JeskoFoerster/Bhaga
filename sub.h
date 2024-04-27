//
// Created by foersterje on 18.04.2024.
//
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <ctype.h>

char* handle_command(Map *map, const char* command);
char** splitByWhiteSpace(const char *longArray, int* numSubarrays);
void handle_client(int client_socket, Map *map);
Map * createSharedMemoryMap();
void writeConnectionMessage(int client_socket);