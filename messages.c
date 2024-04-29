//
// Created by Nils Morczinietz on 29.04.2024.
//

#include "messages.h"

int messageQueueCreate(){
    return msgget(IPC_PRIVATE, IPC_CREAT | 0644);
}

int messageSendPUT(int msg_q_id, char*key, char*value){

    // Nachricht vorbereiten
    struct msg_buffer message;
    message.msg_type = MSG_PUT_TYPE;
    snprintf(message.msg_text, MSG_SIZE, "%s:%s", key, value);

    // Nachricht senden
    int rc = msgsnd(msg_q_id, &message, sizeof(message.msg_text), 0);

    return rc;
}