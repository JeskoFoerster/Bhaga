//
// Created by Jesko Förster on 18.04.2024.
//
#include "map.h"
#include "sub.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#ifndef BHAGA_MAIN_H
#define BHAGA_MAIN_H
char** splitByChar(char longArray[1024], int* numSubarrays, const char* splittChar);

#endif //BHAGA_MAIN_H
