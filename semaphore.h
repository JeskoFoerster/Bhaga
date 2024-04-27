//
// Created by Nils Morczinietz on 27.04.2024.
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int semaphoreCreateGroup(int count);
int semaphoreSetValue(int sem_group_id, int sem_id, int value);
int semaphoreDown(int sem_group_id, int sem_id);
int semaphoreUp(int sem_group_id, int sem_id);