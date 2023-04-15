#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

// Define constants for the number of producers, consumers, and convoys
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define NUM_CONVOIS 200

// Declare semaphores for producers and consumers
sem_t sem_driver, sem_military, sem_material;
sem_t sem_plane, sem_truck, sem_boat;

// Declare mutexes for convoy ID, number of convoys processed, hangar access, and consumers_done flag
pthread_mutex_t mutex_convoi_id, mutex_convois_processed;
pthread_mutex_t mutex_hangar, mutex_consumers_done;

// Define producer types
typedef enum
{
    DRIVER,
    MILITARY,
    MATERIAL
} producer_type;

// Define consumer types
typedef enum
{
    PLANE,
    TRUCK,
    BOAT
} consumer_type;

// Declare global variables for convoy counts, convoy ID, and hangar
int convois_processed = 0;
int convoi_id = 0;
int plane_count = 0;
int truck_count = 0;
int boat_count = 0;
int hangar[6][5] = {0};

// Declare global variables for the done flag and consumers_done count
bool done = false;
int consumers_done = 0;

// Function to print the current state of the hangar
void print_hangar()
{
    printf("PILOTE-MILITARY-MATERIAL-TOTAL-NUMERO\n");
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++)
            printf("%7d", hangar[i][j]);
        printf("\n");
    }
    printf("\n\n");
}
// Global array to hold the counts of drivers, military personnel, and materials
int counts[3] = {0, 0, 0}; // 0: driver_count, 1: military_count, 2: material_count

// Find the first empty row in the hangar for the current column
int find_first_empty_row(int col_index)
{
    for (int i = 0; i < 6; i++)
        if (hangar[i][col_index] == 0)
            return i;
    return -1;
}

// Signal the appropriate semaphore based on the total weight
void signal_semaphore(int total_weight)
{
    if (total_weight < 10001)
        sem_post(&sem_plane); // Signal that a plane is available
    else if (total_weight > 10001 && total_weight < 30001)
        sem_post(&sem_truck); // Signal that a truck is available
    else if (total_weight > 30001)
        sem_post(&sem_boat); // Signal that a boat is available
}

/// @brief Function to place an item in the hangar
/// @param weight
/// @param range
/// @param col_index
void put(int weight, int range, int col_index)
{
    pthread_mutex_lock(&mutex_hangar); // Lock the mutex for the hangar

    // Check if the count for the current column has reached its limit
    if (done)
    {
        pthread_mutex_unlock(&mutex_hangar); // Unlock the mutex and return
        return;
    }

    counts[col_index]++; // Increment the count for the current column
    int first_empty_row = find_first_empty_row(col_index);

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
            // Signal the appropriate semaphore based on the total weight
            signal_semaphore(hangar[first_empty_row][3]);
        }
    }
    // Unlock the mutex for the hangar
    pthread_mutex_unlock(&mutex_hangar);
}

// Helper function to set bounds and increment counts based on the transportation type
void set_bounds_and_increment_counts(const char *name, int *lower_bound, int *upper_bound)
{
    if (strcmp(name, "plane") == 0)
    {
        *lower_bound = 0;
        *upper_bound = 10001;
        plane_count++;
    }
    else if (strcmp(name, "truck") == 0)
    {
        *lower_bound = 10001;
        *upper_bound = 30001;
        truck_count++;
    }
    else if (strcmp(name, "boat") == 0)
    {
        *lower_bound = 30001;
        *upper_bound = INT_MAX;
        boat_count++;
    }
}

// Helper function to find the appropriate convoy
int find_appropriate_convoy(int lower_bound, int upper_bound)
{
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
                return i; // Return the index of the matching convoy
            }
        }
    }
    return -1;
}

/// @brief Function to retrieve and process a convoy based on its transportation type (plane, truck, or boat)
/// @param name
void get(const char *name)
{
    int lower_bound, upper_bound; // Declare lower and upper bounds for transportation type
    set_bounds_and_increment_counts(name, &lower_bound, &upper_bound);

    pthread_mutex_lock(&mutex_hangar); // Lock the mutex for the hangar

    int matching_convoy_index = find_appropriate_convoy(lower_bound, upper_bound);

    // Check if a matching convoy was found
    if (matching_convoy_index != -1)
    {
        // Clear the hangar for the convoy
        memset(hangar[matching_convoy_index], 0, 5 * sizeof(int));

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

        // print_hangar();                        // Print the hangar
    }

    // Release the appropriate semaphore
    sem_post(&sem_driver);
    sem_post(&sem_military);
    sem_post(&sem_material);
    return NULL; // Return NULL as the function return value
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

/// @brief main
/// @return
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
    for (int i = 0; i < NUM_PRODUCERS; ++i)
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
    for (int i = 0; i < NUM_PRODUCERS; ++i)
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
    for (int i = 0; i < NUM_PRODUCERS; ++i)
    {
        sem_destroy(producer_sems[i]);
        sem_destroy(consumer_sems[i]);
    }

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
