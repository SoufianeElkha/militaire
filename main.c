/*
    @author: Soufiane El kharmoudi
    @date: 16-03-2024
    @brief: tp4 military convoy
    Implémenter un système de producteurs – consommateurs concurrents et asynchrones en respectant les contraintes de développement. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include "header/producer.h"
#include "header/consumer.h"
#include "header/shared_resources.h"

/// @brief main
/// @return 0
int main()
{
    // Initialize mutexes
    int init_result;

    init_result = pthread_mutex_init(&mutex_convoi_id, NULL);
    if (init_result != 0)
    {
        printf("Error initializing mutex_convoi_id. Error code: %d\n", init_result);
        exit(EXIT_FAILURE);
    }

    init_result = pthread_mutex_init(&mutex_convois_processed, NULL);
    if (init_result != 0)
    {
        printf("Error initializing mutex_convois_processed. Error code: %d\n", init_result);
        exit(EXIT_FAILURE);
    }

    init_result = pthread_mutex_init(&mutex_hangar, NULL);
    if (init_result != 0)
    {
        printf("Error initializing mutex_hangar. Error code: %d\n", init_result);
        exit(EXIT_FAILURE);
    }

    init_result = pthread_mutex_init(&mutex_consumers_done, NULL);
    if (init_result != 0)
    {
        printf("Error initializing mutex_consumers_done. Error code: %d\n", init_result);
        exit(EXIT_FAILURE);
    }

    // Declare arrays for semaphores, thread names, and thread handles
    sem_t *producer_sems[NUM_PRODUCERS] = {&sem_driver, &sem_military, &sem_material};
    sem_t *consumer_sems[NUM_CONSUMERS] = {&sem_plane, &sem_truck, &sem_boat};

    const char *producer_names[] = {"driver", "military", "material"};
    const char *consumer_names[] = {"plane", "truck", "boat"};

    pthread_t producer_threads[NUM_PRODUCERS];
    pthread_t consumer_threads[NUM_CONSUMERS];

    // Initialize semaphores and create producer and consumer threads
    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        // Initialize producer semaphore
        if (sem_init(producer_sems[i], 0, 6) != 0)
        {
            printf("Error initializing semaphore for %s producer\n", producer_names[i]);
            exit(EXIT_FAILURE);
        }

        // Initialize consumer semaphore
        if (sem_init(consumer_sems[i], 0, 0) != 0)
        {
            printf("Error initializing semaphore for %s consumer\n", consumer_names[i]);
            exit(EXIT_FAILURE);
        }

        // Create producer thread
        int producer_thread_creation = pthread_create(&producer_threads[i], NULL, producer, (void *)producer_names[i]);
        if (producer_thread_creation != 0)
        {
            printf("Error creating %s producer thread. Error code: %d\n", producer_names[i], producer_thread_creation);
            exit(EXIT_FAILURE);
        }

        // Create consumer thread
        int consumer_thread_creation = pthread_create(&consumer_threads[i], NULL, consumer, (void *)consumer_names[i]);
        if (consumer_thread_creation != 0)
        {
            printf("Error creating %s consumer thread. Error code: %d\n", consumer_names[i], consumer_thread_creation);
            exit(EXIT_FAILURE);
        }
    }

    // Join producer and consumer threads
    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        int producer_join_result = pthread_join(producer_threads[i], NULL);
        if (producer_join_result != 0)
        {
            printf("Error joining %s producer thread. Error code: %d\n", producer_names[i], producer_join_result);
            exit(EXIT_FAILURE);
        }

        int consumer_join_result = pthread_join(consumer_threads[i], NULL);
        if (consumer_join_result != 0)
        {
            printf("Error joining %s consumer thread. Error code: %d\n", consumer_names[i], consumer_join_result);
            exit(EXIT_FAILURE);
        }
    }

    // Destroy producer and consumer semaphores
    sem_destroy(producer_sems[0]);
    sem_destroy(producer_sems[1]);
    sem_destroy(producer_sems[2]);
    sem_destroy(consumer_sems[0]);
    sem_destroy(consumer_sems[1]);
    sem_destroy(consumer_sems[2]);

    // Destroy mutexes
    pthread_mutex_destroy(&mutex_convoi_id);
    pthread_mutex_destroy(&mutex_convois_processed);
    pthread_mutex_destroy(&mutex_hangar);
    pthread_mutex_destroy(&mutex_consumers_done);

    // Print results
    printf("Convoys executed: %d\n", convois_processed);
    printf("Planes dispatched: %d\n", plane_count);
    printf("Trucks dispatched: %d\n", truck_count);
    printf("Boats dispatched: %d\n", boat_count);
    printf("Total Planes + Trucks + Boats dispatched: %d\n", plane_count + truck_count + boat_count);

    return 0;
}
