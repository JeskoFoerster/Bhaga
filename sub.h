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
#include "semaphore.h"
#include <stdbool.h>
#include "messages.h"
#include "subscriptions.h"

char* handle_command(Map *map, const char* command, int sem_group_id, bool* inTransaction, int msg_q_id, int * msg_q_ids, SubscriptionArray * sub_list);
char** splitByWhiteSpace(const char *longArray, int* numSubarrays);
void handle_client(int client_socket, Map *map, int sem_group_id, bool* inTransaction, int msg_q_id, int * msg_q_ids);
void writeConnectionMessage(int client_socket);