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

/**
 * Handles communication with a client socket, processing commands and sending responses.
 *
 * @param client_socket The file descriptor of the client socket.
 * @param map A pointer to the Map structure.
 * @param sem_group_id The semaphore group ID.
 * @param inTransaction A pointer to a boolean indicating whether a transaction is in progress.
 * @param msg_q_id The ID of the message queue.
 * @param msg_q_ids An array of message queue IDs.
 */
char* handle_command(Map *map, const char* command, int sem_group_id, bool* inTransaction, int msg_q_id, int * msg_q_ids, SubscriptionArray * sub_list);

/**
 * Splits a command string into subarrays based on whitespace.
 *
 * @param command The command string to split.
 * @param numSubarrays A pointer to an integer to store the number of subarrays created.
 * @return An array of subarrays containing the command parts.
 */
char** splitByWhiteSpace(const char *longArray, int* numSubarrays);

/**
 * Handles a command received from the client.
 *
 * @param map A pointer to the Map structure.
 * @param command The command string.
 * @param sem_group_id The semaphore group ID.
 * @param inTransaction A pointer to a boolean indicating whether a transaction is in progress.
 * @param msg_q_id The ID of the message queue.
 * @param msg_q_ids An array of message queue IDs.
 * @param sub_list A pointer to the SubscriptionArray structure.
 * @return The response to the command.
 */
void handle_client(int client_socket, Map *map, int sem_group_id, bool* inTransaction, int msg_q_id, int * msg_q_ids);

/**
 * Writes an overview of available commands to the client socket.
 *
 * @param client_socket The file descriptor of the client socket.
 */
void writeConnectionMessage(int client_socket);