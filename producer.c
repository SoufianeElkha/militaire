#include "producer.h"
#include "shared_resources.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>


/// @brief Function to place an item in the hangar
/// @param weight
/// @param range
/// @param col_index
void put(int weight, int range, int col_index)
{
    int first_empty_row = -1; // Initialize first empty row as -1
    // Array to hold the counts of drivers, military personnel, and materials
    static int counts[3] = {0, 0, 0}; // 0: driver_count, 1: military_count, 2: material_count

    pthread_mutex_lock(&mutex_hangar); // Lock the mutex for the hangar

    // Check if the count for the current column has reached its limit
    if (done)
    {
        pthread_mutex_unlock(&mutex_hangar); // Unlock the mutex and return
        return;
    }
    counts[col_index]++; // Increment the count for the current column

    // Find the first empty row in the hangar for the current column
    for (int i = 0; i < 6; i++)
    {
        if (hangar[i][0] == 0 || hangar[i][1] == 0 || hangar[i][2] == 0)
        {
            if (hangar[i][col_index] == 0)
            {
                first_empty_row = i;
                break;
            }
        }
    }

    // If an empty row is found
    if (first_empty_row != -1)
    {
        // Generate a random number within the specified range
        int random_num = rand() % (2 * range + 1) - range;
        // Assign the weight with the random number added to the hangar
        hangar[first_empty_row][col_index] = weight + random_num;

        // Check if the current row is completely filled
        if (hangar[first_empty_row][0] != 0 && hangar[first_empty_row][1] != 0 && hangar[first_empty_row][2] != 0)
        {
            // Calculate the total weight for the current row
            hangar[first_empty_row][3] = hangar[first_empty_row][0] + hangar[first_empty_row][1] + hangar[first_empty_row][2];

            // Lock the mutex for convoi_id
            pthread_mutex_lock(&mutex_convoi_id);
            // Increment and assign the convoi_id
            hangar[first_empty_row][4] = ++convoi_id;
            // Unlock the mutex for convoi_id
            pthread_mutex_unlock(&mutex_convoi_id);

            // Determine the type of transportation for the current row based on the total weight
            if (hangar[first_empty_row][3] < 10001)
            {
                sem_post(&sem_plane); // Signal that a plane is available
            }
            else if (hangar[first_empty_row][3] > 10001 && hangar[first_empty_row][3] < 30001)
            {
                sem_post(&sem_truck); // Signal that a truck is available
            }
            else if (hangar[first_empty_row][3] > 30001)
            {
                sem_post(&sem_boat); // Signal that a boat is available
            }
        }
    }
    // Unlock the mutex for the hangar
    pthread_mutex_unlock(&mutex_hangar);
}

/// @brief Function for the producer threads (driver, military, and material)
/// @param arg
/// @return
void *producer(void *arg)
{
    char *name = (char *)arg; // Get the producer name from the argument
    producer_type p_type;     // Declare the producer type

    // Set the producer type based on the name
    if (strcmp(name, "driver") == 0)
        p_type = DRIVER;
    else if (strcmp(name, "military") == 0)
        p_type = MILITARY;
    else if (strcmp(name, "material") == 0)
        p_type = MATERIAL;
    else
        return NULL; // Return NULL if the producer type is not recognized

    // Continue running until the 'done' flag is set
    while (!done)
    {
        int weight, range, col_index; // Declare weight, range, and column index variables
        sem_t *semaphore;             // Declare the semaphore pointer

        // Set weight, range, column index, and semaphore based on the producer type
        switch (p_type)
        {
        case DRIVER:
            weight = 80;
            range = 20;
            col_index = 0;
            semaphore = &sem_driver;
            break;
        case MILITARY:
            weight = 8000;
            range = 7000;
            col_index = 1;
            semaphore = &sem_military;
            break;
        case MATERIAL:
            weight = 25000;
            range = 24500;
            col_index = 2;
            semaphore = &sem_material;
            break;
        }

        // Check if the maximum number of convoys has been reached
        if (convois_processed >= NUM_CONVOIS)
        {
            pthread_mutex_lock(&mutex_consumers_done);
            done = true; // Set the 'done' flag
            pthread_mutex_unlock(&mutex_consumers_done);
            break; // Break the loop
        }
        else
        {
            sem_wait(semaphore); // Wait for the semaphore

            // Check if the maximum number of convoys has been reached while waiting for the semaphore
            pthread_mutex_lock(&mutex_convois_processed);
            if (convois_processed >= NUM_CONVOIS)
            {
                pthread_mutex_unlock(&mutex_convois_processed);
                sem_post(semaphore); // Release the semaphore
                break;               // Break the loop
            }
            pthread_mutex_unlock(&mutex_convois_processed);

            put(weight, range, col_index); // Call the 'put' function with the calculated weight, range, and column index
        }
    }

    // Release the appropriate semaphore
    sem_post(&sem_driver);
    sem_post(&sem_military);
    sem_post(&sem_material);
    return NULL; // Return NULL as the function return value
}
