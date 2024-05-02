//
// Created by Nils Morczinietz on 29.04.2024.
//

#ifndef BHAGA_MESSAGES_H
#define BHAGA_MESSAGES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "subscriptions.h"

#define MSG_PUT_TYPE 11
#define MSG_SIZE 128

// Struktur für die Nachricht
struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

/**
 * Creates a new message queue or returns the ID of an existing one.
 *
 * @param msg_q_id The ID of the message queue.
 * @return The ID of the message queue created or accessed.
 */
int messageQueueCreate();

/**
 * Sends a PUT message to a specified message queue.
 *
 * @param msg_q_id The ID of the message queue to send the message to.
 * @param key The key part of the message.
 * @param value The value part of the message.
 * @return Returns 0 on success, or -1 on failure.
 */
int messageSendPUT(int msg_q_id, char*key, char*value);

/**
 * Sends a PUT message to all specified message queues except the one with the own_msg_q_id.
 *
 * @param own_msg_q_id The ID of the message queue to exclude from sending.
 * @param msg_q_ids Array of message queue IDs to send the message to.
 * @param key The key part of the message.
 * @param value The value part of the message.
 * @return Returns 0 on success, or -1 on failure.
 */
int messageSendToAllPUT(int own_msg_q_id, int * msg_q_ids, char*key, char*value);

/**
 * Prints information about the specified message queue.
 *
 * @param msg_q_id The ID of the message queue.
 */
void messageQueueInfo(int msg_q_id);

/**
 * Receives the content of a message from the specified message queue and returns it.
 *
 * @param msg_q_id The ID of the message queue.
 * @param sub_list A pointer to the SubscriptionArray to check subscription.
 * @return The content of the received message, or an empty string if no message is received or the key is not subscribed.
 */
char* receiveMessageContent(int msg_q_id, SubscriptionArray * sub_list);

/**
 * Splits a message into key and value parts.
 *
 * @param message The message to split.
 * @param key A pointer to store the key part.
 * @param value A pointer to store the value part.
 */
void splitMessage(const char *message, char **key, char **value);

#endif //BHAGA_MESSAGES_H
