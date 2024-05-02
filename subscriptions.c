//
// Created by Nils Morczinietz on 02.05.2024.
//

#include "subscriptions.h"

SubscriptionArray * createSharedSubscriptionArray() {
    /*
    SubscriptionArray * newArray = (SubscriptionArray *)malloc(sizeof(SubscriptionArray));
    if (newArray == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newArray->count = 0;
    return newArray;
     */
    // Erstellen des Shared Memory
    int shmid = shmget(IPC_PRIVATE, sizeof(SubscriptionArray), IPC_CREAT|0600);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    SubscriptionArray * sub_list = (SubscriptionArray *)shmat(shmid, 0, 0);
    if (sub_list == (void *)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    return sub_list;
}

void addSubscription(SubscriptionArray * array, const char * subscription) {
    if (array->count < MAX_SUBSCRIPTIONS) {
        strcpy(array->subscriptions[array->count], subscription);
        array->count++;
    } else {
        printf("Max subscriptions reached. Cannot add more.\n");
    }
}

void removeSubscription(SubscriptionArray * array, const char * subscription) {
    for (int i = 0; i < array->count; ++i) {
        if (strcmp(array->subscriptions[i], subscription) == 0) {
            // Replace the found subscription with the last subscription in the array
            strcpy(array->subscriptions[i], array->subscriptions[array->count - 1]);
            array->count--;
            printf("Subscription '%s' removed.\n", subscription);
            return;
        }
    }
    printf("Subscription '%s' not found.\n", subscription);
}

void clearSubscriptions(SubscriptionArray * array) {
    array->count = 0;
    printf("All subscriptions cleared.\n");
}

int isSubscribed(SubscriptionArray * array, const char * subscription) {
    for (int i = 0; i < array->count; ++i) {
        if (strcmp(array->subscriptions[i], subscription) == 0) {
            return 1; // Subscription found
        }
    }
    return 0; // Subscription not found
}