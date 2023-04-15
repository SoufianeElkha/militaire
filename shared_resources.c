#include "header/shared_resources.h"

// Global variables
int convois_processed = 0;
int plane_count = 0;
int truck_count = 0;
int boat_count = 0;

// Declare global variables for convoy counts, convoy ID, and hangar
int convoi_id = 0;
int hangar[6][5] = {0};

// Declare global variables for the done flag and consumers_done count
bool done = false;
int consumers_done = 0;

// Mutexes
pthread_mutex_t mutex_convoi_id;
pthread_mutex_t mutex_convois_processed;
pthread_mutex_t mutex_hangar;
pthread_mutex_t mutex_consumers_done;

// Semaphores
sem_t sem_driver;
sem_t sem_military;
sem_t sem_material;
sem_t sem_plane;
sem_t sem_truck;
sem_t sem_boat;
