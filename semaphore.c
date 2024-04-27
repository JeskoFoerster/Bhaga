//
// Created by Nils Morczinietz on 27.04.2024.
//

//TEST

#include "semaphore.h"

int semaphoreCreateGroup(int count){
    int sem_group_id = semget (IPC_PRIVATE, count, IPC_CREAT|0644);
    return sem_group_id;
}

int semaphoreSetValue(int sem_group_id, int sem_id, int value){
    unsigned short marker[1];
    marker[sem_id] = value;
    int result = semctl(sem_group_id, value, SETALL, marker);
    return result;
}

int semaphoreDown(int sem_group_id, int sem_id){
    struct sembuf values;
    values.sem_num = sem_id;
    values.sem_flg = SEM_UNDO;
    values.sem_op = -1;
    int result = semop(sem_group_id, &values, 1);
    return result;
}

int semaphoreUp(int sem_group_id, int sem_id){
    struct sembuf values;
    values.sem_num = sem_id;
    values.sem_flg = SEM_UNDO;
    values.sem_op = 1;
    int result = semop(sem_group_id, &values, 1);
    return result;
}