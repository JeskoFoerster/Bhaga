//
// Created by Nils Morczinietz on 29.04.2024.
//

#include "messages.h"
#define MAX_CLIENTS 5

int messageQueueCreate(int msg_q_id){
    return msgget(IPC_PRIVATE, IPC_CREAT | 0644);
}

void messageQueueInfo(int msg_q_id) {
    struct msqid_ds queue_info;

    // Informationen über die Message Queue abrufen
    if (msgctl(msg_q_id, IPC_STAT, &queue_info) == -1) {
        perror("Fehler beim Abrufen der Informationen über die Message Queue");
        exit(EXIT_FAILURE);
    }

    // Informationen ausgeben
    printf("Message Queue ID: %d\n", msg_q_id);
    printf("Key: %d\n", queue_info.msg_perm.__key);
    printf("Owner UID: %d\n", queue_info.msg_perm.uid);
    printf("Owner GID: %d\n", queue_info.msg_perm.gid);
    printf("Creator UID: %d\n", queue_info.msg_perm.cuid);
    printf("Creator GID: %d\n", queue_info.msg_perm.cgid);
    printf("Access Permissions: %o\n", queue_info.msg_perm.mode);
    printf("Message Queue Size (bytes): %ld\n", queue_info.msg_qbytes);
    printf("Number of Messages in Queue: %ld\n", queue_info.msg_qnum);
    printf("Last Send Time: %ld\n", queue_info.msg_stime);
    printf("Last Receive Time: %ld\n", queue_info.msg_rtime);
    printf("Last Change Time: %ld\n", queue_info.msg_ctime);
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

int messageSendToAllPUT(int own_msg_q_id, int * msg_q_ids, char*key, char*value){

    // Nachricht vorbereiten
    struct msg_buffer message;
    message.msg_type = MSG_PUT_TYPE;
    snprintf(message.msg_text, MSG_SIZE, "UPDATE %s %s\n\r", key, value);

    // Nachricht senden
    int i = 0;
    while(i < MAX_CLIENTS && msg_q_ids[i]!=0){
        if(msg_q_ids[i] == own_msg_q_id){
            i++;
            continue;
        }
        printf("ID: %i", msg_q_ids[i]);
        int rc = msgsnd(msg_q_ids[i], &message, sizeof(message.msg_text), 0);
        if(rc < 0){
            perror("Fehler beim versenden einer PUT nachricht");
            exit(1);
        }
        printf("Send: %s", message.msg_text);
        i++;
    }

    return 0;
}

char* receiveMessageContent(int msg_q_id) {
    // Nachricht empfangen
    struct msg_buffer message;
    int rc = msgrcv(msg_q_id, &message, sizeof(message.msg_text), MSG_PUT_TYPE, IPC_NOWAIT);
    if (rc < 0) {
        return "";
    }

    // Speicher für den Nachrichteninhalt dynamisch zuweisen und kopieren
    char *content = (char *)malloc(strlen(message.msg_text) + 1);
    if (content == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    strcpy(content, message.msg_text);

    return content;
}