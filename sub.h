//
// Created by foersterje on 18.04.2024.
//
#include "map.h"

char* handle_command(Map *map, const char* command);
char** splitByWhiteSpace(const char *longArray, int* numSubarrays);
void handle_client(int client_socket, Map *map);

