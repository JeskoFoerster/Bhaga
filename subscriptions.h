//
// Created by Nils Morczinietz on 02.05.2024.
//

#ifndef BHAGA_SUBSCRIPTIONS_H
#define BHAGA_SUBSCRIPTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

#define MAX_SUBSCRIPTIONS 100
#define MAX_SUBSCRIPTION_LENGTH 50

typedef struct {
    char subscriptions[MAX_SUBSCRIPTIONS][MAX_SUBSCRIPTION_LENGTH];
    int count;
} SubscriptionArray;

/**
 * Creates a shared subscription array in shared memory.
 *
 * @return A pointer to the shared SubscriptionArray.
 */
SubscriptionArray * createSharedSubscriptionArray();

/**
 * Adds a subscription to the SubscriptionArray.
 *
 * @param array A pointer to the SubscriptionArray.
 * @param subscription The subscription to add.
 */
void addSubscription(SubscriptionArray * array, const char * subscription);

/**
 * Removes a subscription from the SubscriptionArray.
 *
 * @param array A pointer to the SubscriptionArray.
 * @param subscription The subscription to remove.
 */
void removeSubscription(SubscriptionArray * array, const char * subscription);

/**
 * Clears all subscriptions from the SubscriptionArray.
 *
 * @param array A pointer to the SubscriptionArray.
 */
void clearSubscriptions(SubscriptionArray * array);

/**
 * Checks if a subscription exists in the SubscriptionArray.
 *
 * @param sub_list A pointer to the SubscriptionArray to search.
 * @param subscription The subscription to search for.
 * @return 1 if the subscription is found, otherwise 0.
 */
int isSubscribed(SubscriptionArray * array, const char * subscription);

#endif //BHAGA_SUBSCRIPTIONS_H
