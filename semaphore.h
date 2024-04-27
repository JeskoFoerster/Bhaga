//
// Created by Nils Morczinietz on 27.04.2024.
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Creates a semaphore group with the specified number of semaphores.
 * @param count The number of semaphores to create in the group.
 * @return The semaphore group ID on success, -1 on error.
 */
int semaphoreCreateGroup(int count);

/**
 * Initializes the values of all semaphores in a semaphore group.
 * @param sem_group_id The ID of the semaphore group.
 * @param sem_id Ignored (for compatibility with future extensions).
 * @param value The initial value to set for all semaphores in the group.
 * @return 0 on success, -1 on error.
 */
int semaphoreSetValue(int sem_group_id, int sem_id, int value);

/**
 * Decrements the value of a specific semaphore in a semaphore group,
 * potentially blocking the calling process until the value becomes non-negative.
 *
 * @param sem_group_id The ID of the semaphore group.
 * @param sem_id The index of the semaphore within the group to decrement.
 * @return 0 on success, -1 on error.
 */
int semaphoreDown(int sem_group_id, int sem_id);

/**
 * Increments the value of a specific semaphore in a semaphore group.
 * @param sem_group_id The ID of the semaphore group.
 * @param sem_id The index of the semaphore within the group to increment.
 * @return 0 on success, -1 on error.
 */
int semaphoreUp(int sem_group_id, int sem_id);