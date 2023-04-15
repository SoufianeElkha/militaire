#ifndef CONSUMER_H
#define CONSUMER_H

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
