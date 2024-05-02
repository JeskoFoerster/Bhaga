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
 * This function creates a new message queue and returns its identifier.
 *
 * @return Returns the identifier of the new message queue on success, or -1 on failure.
 */
int messageQueueCreate();

/**
 * This function prepares a PUT message with the provided key and value,
 * then sends it to the specified message queue with PUT operation.
 *
 * @param msg_q_id The message queue identifier.
 * @param key The key for the message.
 * @param value The value for the message.
 * @return Returns 0 on success, -1 on failure.
 */
int messageSendPUT(int msg_q_id, char*key, char*value);

int messageSendToAllPUT(int own_msg_q_id, int * msg_q_ids, char*key, char*value);

void messageQueueInfo(int msg_q_id);

char* receiveMessageContent(int msg_q_id, SubscriptionArray * sub_list);

void splitMessage(const char *message, char **key, char **value);

#endif //BHAGA_MESSAGES_H
