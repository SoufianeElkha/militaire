#ifndef PRODUCER_H
#define PRODUCER_H

#include <string.h>
#include "shared_resources.h"

// Define producer types
typedef enum
{
    DRIVER,
    MILITARY,
    MATERIAL
} producer_type;

// Function declarations
void put(int weight, int range, int col_index);
void *producer(void *arg);

#endif // PRODUCER_H
