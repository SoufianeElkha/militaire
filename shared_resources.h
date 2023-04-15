#ifndef SHARED_RESOURCES_H
#define SHARED_RESOURCES_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// Define constants for the number of producers, consumers, and convoys
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define NUM_CONVOIS 200

// Global variables
extern int convois_processed;
extern int plane_count;
extern int truck_count;
extern int boat_count;

// Declare global variables for convoy counts, convoy ID, and hangar
extern int convoi_id;
extern int hangar[6][5];

// Declare global variables for the done flag and consumers_done count
extern bool done;
extern int consumers_done;

// Mutexes
extern pthread_mutex_t mutex_convoi_id;
extern pthread_mutex_t mutex_convois_processed;
extern pthread_mutex_t mutex_hangar;
extern pthread_mutex_t mutex_consumers_done;

// Semaphores
extern sem_t sem_driver;
extern sem_t sem_military;
extern sem_t sem_material;
extern sem_t sem_plane;
extern sem_t sem_truck;
extern sem_t sem_boat;

#endif // SHARED_RESOURCES_H
