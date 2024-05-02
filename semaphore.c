//
// Created by Nils Morczinietz on 27.04.2024.
//

#include "semaphore.h"

int semaphore_create_group(int count){
    int sem_group_id = semget (IPC_PRIVATE, count, IPC_CREAT|0644);
    return sem_group_id;
}

int semaphore_set_value(int sem_group_id, int sem_id, int value){
    struct sembuf values;
    values.sem_num = sem_id;
    values.sem_op = value;
    values.sem_flg = 0;
    int result = semop(sem_group_id, &values, 1);
    return result;
}

int semaphore_down(int sem_group_id, int sem_id){
    struct sembuf values;
    values.sem_num = sem_id;
    values.sem_flg = SEM_UNDO;
    values.sem_op = -1;
    int result = semop(sem_group_id, &values, 1);
    return result;
}

int semaphore_up(int sem_group_id, int sem_id){
    struct sembuf values;
    values.sem_num = sem_id;
    values.sem_flg = SEM_UNDO;
    values.sem_op = 1;
    int result = semop(sem_group_id, &values, 1);
    return result;
}