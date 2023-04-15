#include "consumer.h"
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


/// @brief Function to retrieve and process a convoy based on its transportation type (plane, truck, or boat)
/// @param name
void get(const char *name)
{
    int tmp = -1;                 // Initialize temporary index as -1
    int lower_bound, upper_bound; // Declare lower and upper bounds for transportation type

    // Set the bounds and increment counts based on the transportation type
    if (strcmp(name, "plane") == 0)
    {
        lower_bound = 0;
        upper_bound = 10001;
        plane_count++;
    }
    else if (strcmp(name, "truck") == 0)
    {
        lower_bound = 10001;
        upper_bound = 30001;
        truck_count++;
    }
    else if (strcmp(name, "boat") == 0)
    {
        lower_bound = 30001;
        upper_bound = INT_MAX;
        boat_count++;
    }
    else
    {
        return; // Return if the transportation type is not recognized
    }

    pthread_mutex_lock(&mutex_hangar); // Lock the mutex for the hangar

    // Find the appropriate convoy
    for (int i = 0; i < 6; i++)
    {
        // Check if the row is complete
        if (hangar[i][0] != 0 && hangar[i][1] != 0 && hangar[i][2] != 0)
        {
            int current_weight = hangar[i][3];    // Get the current weight
            int current_convoi_nb = hangar[i][4]; // Get the current convoy number
            // Check if the convoy falls within the bounds for the given transportation type
            if (current_weight >= lower_bound && current_weight < upper_bound && current_convoi_nb <= NUM_CONVOIS && current_convoi_nb > 0)
            {
                tmp = i; // Store the index of the matching convoy
            }
        }
    }

    // Check if a matching convoy was found
    if (tmp != -1)
    {
        // Clear the hangar for the convoy
        memset(hangar[tmp], 0, 5 * sizeof(int));

        // Release the semaphores
        sem_post(&sem_driver);
        sem_post(&sem_military);
        sem_post(&sem_material);

        // Increase the count of processed convoys
        pthread_mutex_lock(&mutex_convois_processed); // Lock the mutex for convois_processed
        convois_processed++;
        pthread_mutex_unlock(&mutex_convois_processed); // Unlock the mutex for convois_processed
    }

    pthread_mutex_unlock(&mutex_hangar); // Unlock the mutex for the hangar
}

/// @brief Function for the consumer threads (plane, truck, and boat)
/// @param arg
/// @return
void *consumer(void *arg)
{
    const char *name = (const char *)arg; // Get the consumer name from the argument
    consumer_type c_type;                 // Declare the consumer type

    // Set the consumer type based on the name
    if (strcmp(name, "plane") == 0)
        c_type = PLANE;
    else if (strcmp(name, "truck") == 0)
        c_type = TRUCK;
    else if (strcmp(name, "boat") == 0)
        c_type = BOAT;
    else
        return NULL; // Return NULL if the consumer type is not recognized

    // Continue running until the 'done' flag is set
    while (!done)
    {
        sem_t *semaphore;   // Declare the semaphore pointer
        const char *c_name; // Declare the consumer name

        // Set the semaphore and consumer name based on the consumer type
        switch (c_type)
        {
        case PLANE:
            semaphore = &sem_plane;
            c_name = "plane";
            break;
        case TRUCK:
            semaphore = &sem_truck;
            c_name = "truck";
            break;
        case BOAT:
            semaphore = &sem_boat;
            c_name = "boat";
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

            get(c_name); // Call the 'get' function with the consumer name
        }
    }

    // Release the appropriate semaphore
    sem_post(&sem_plane);
    sem_post(&sem_truck);
    sem_post(&sem_boat);

    return NULL; // Return NULL as the function return value
}
