//
// Created by foersterje on 18.04.2024.
//

#ifndef BHAGA_SUB_H
#define BHAGA_SUB_H
#include "map.h"

#ifndef BHAGA_MAIN_H
#define BHAGA_MAIN_H
int create_command_line_interface();
int create_socket();
char** splitByChar(char longArray[1024], int* numSubarrays, const char* splittChar);

#endif //BHAGA_MAIN_H
#endif //BHAGA_SUB_H
