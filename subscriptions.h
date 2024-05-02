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

SubscriptionArray * createSharedSubscriptionArray();
void addSubscription(SubscriptionArray * array, const char * subscription);
void removeSubscription(SubscriptionArray * array, const char * subscription);
void printSubscriptions(SubscriptionArray * array);
void clearSubscriptions(SubscriptionArray * array);
int isSubscribed(SubscriptionArray * array, const char * subscription);

#endif //BHAGA_SUBSCRIPTIONS_H
