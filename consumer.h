#ifndef CONSUMER_H
#define CONSUMER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <semaphore.h>
#include <pthread.h>

// Define consumer types
typedef enum
{
    PLANE,
    TRUCK,
    BOAT
} consumer_type;

// Function declarations
void get(const char *name);
void *consumer(void *arg);

#endif // CONSUMER_H
