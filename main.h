//
// Created by Jesko Förster on 18.04.2024.
//
#include "map.h"
#include "sub.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PORT 5678
#define MAX_CLIENTS 5

typedef struct {
    int client_socket;
    Map *map;
} ClientData;