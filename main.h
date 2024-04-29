//
// Created by Jesko Förster on 18.04.2024.
//
#include "map.h"
#include "sub.h"
#include "semaphore.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include "messages.h"