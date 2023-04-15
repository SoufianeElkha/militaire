#ifndef CONSUMER_H
#define CONSUMER_H

#include <string.h>
#include <limits.h>
#include "shared_resources.h"
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
