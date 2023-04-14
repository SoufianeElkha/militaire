#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define NUM_CONVOIS 200

sem_t sem_driver, sem_military, sem_material;
sem_t sem_plane, sem_truck, sem_boat;

pthread_mutex_t mutex_convoi_id, mutex_convois_processed;
pthread_mutex_t mutex_buffer;

typedef enum
{
    DRIVER,
    MILITARY,
    MATERIAL
} producer_type;

typedef enum
{
    PLANE,
    TRUCK,
    BOAT
} consumer_type;

int convoi_id = 0;
int convois_processed = 0;
int buffer[6][5] = {0};
bool done = false;
int consumers_done = 0;
pthread_mutex_t mutex_consumers_done;

void put(int weight, int range, int col_index)
{
    int first_empty_row = -1;
    static int counts[3] = {0, 0, 0}; // 0: driver_count, 1: military_count, 2: material_count
    int timeToWait = (rand() % 2 == 0) ? 8 : 12;
    usleep(1000 * timeToWait);
    pthread_mutex_lock(&mutex_buffer);

    if (counts[col_index] >= 218)
    {
        pthread_mutex_unlock(&mutex_buffer);
        return;
    }
    counts[col_index]++;

    for (int i = 0; i < 6; i++)
    {
        if (buffer[i][0] == 0 || buffer[i][1] == 0 || buffer[i][2] == 0)
        {
            if (buffer[i][col_index] == 0)
            {
                first_empty_row = i;
                break;
            }
        }
    }

    if (first_empty_row != -1)
    {
        int random_num = rand() % (2 * range + 1) - range;
        buffer[first_empty_row][col_index] = weight + random_num;

        if (buffer[first_empty_row][0] != 0 && buffer[first_empty_row][1] != 0 && buffer[first_empty_row][2] != 0)
        {
            buffer[first_empty_row][3] = buffer[first_empty_row][0] + buffer[first_empty_row][1] + buffer[first_empty_row][2];

            pthread_mutex_lock(&mutex_convoi_id);
            buffer[first_empty_row][4] = ++convoi_id;
            pthread_mutex_unlock(&mutex_convoi_id);

            if (buffer[first_empty_row][3] < 10001)
            {
                sem_post(&sem_plane);
            }
            else if (buffer[first_empty_row][3] > 10001 && buffer[first_empty_row][3] < 30001)
            {
                sem_post(&sem_truck);
            }
            else if (buffer[first_empty_row][3] > 30001)
            {
                sem_post(&sem_boat);
            }
        }
    }
    pthread_mutex_unlock(&mutex_buffer);
}

void get(const char *name)
{
    int tmp_i = -1;
    int lower_bound, upper_bound;

    if (strcmp(name, "plane") == 0)
    {
        lower_bound = 0;
        upper_bound = 10001;
    }
    else if (strcmp(name, "truck") == 0)
    {
        lower_bound = 10001;
        upper_bound = 30001;
    }
    else if (strcmp(name, "boat") == 0)
    {
        lower_bound = 30001;
        upper_bound = INT_MAX;
    }
    else
    {
        return;
    }

    pthread_mutex_lock(&mutex_buffer);

    // Find the appropriate convoy
    for (int i = 0; i < 6; i++)
    {
        if (buffer[i][0] != 0 && buffer[i][1] != 0 && buffer[i][2] != 0) // Check if the row is complete
        {
            int current_weight = buffer[i][3];
            int current_convoi_nb = buffer[i][4];
            if (current_weight >= lower_bound && current_weight < upper_bound && current_convoi_nb <= NUM_CONVOIS && current_convoi_nb > 0)
            {
                tmp_i = i;
            }
        }
    }

    // Check if a matching convoy was found
    if (tmp_i != -1)
    {
        // Clear the buffer for the convoy
        memset(buffer[tmp_i], 0, 5 * sizeof(int));

        // Release the semaphores
        sem_post(&sem_driver);
        sem_post(&sem_military);
        sem_post(&sem_material);

        // Increase the count of processed convoys
        pthread_mutex_lock(&mutex_convois_processed);
        convois_processed++;
        printf("\n######################\nConvoi terminati: %d\n######################\n", convois_processed);
        pthread_mutex_unlock(&mutex_convois_processed);
    }

    pthread_mutex_unlock(&mutex_buffer);
}

void *consumer(void *arg)
{
    const char *name = (const char *)arg;
    consumer_type c_type;

    if (strcmp(name, "plane") == 0)
        c_type = PLANE;
    else if (strcmp(name, "truck") == 0)
        c_type = TRUCK;
    else if (strcmp(name, "boat") == 0)
        c_type = BOAT;
    else
        return NULL;

    while (!done)
    {
        sem_t *semaphore;
        const char *c_name;

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

        if (convois_processed >= NUM_CONVOIS)
        {
            done = true;
            printf("NUMERO MASSIMO RAGGIUNTO\n");
            break;
        }

        sem_wait(semaphore);
        get(c_name);

        // Release the mutex after accessing the buffer
        pthread_mutex_unlock(&mutex_buffer);

        printf("%s is going\n", c_name);
    }
    // Release the appropriate semaphore
    sem_post(&sem_plane);
    sem_post(&sem_truck);
    sem_post(&sem_boat);

    return NULL;
}

void print_buffer()
{
    printf("PILOTE-MILITARY-MATERIAL-TOTAL-NUMERO\n");
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++)
            printf("%7d", buffer[i][j]);
        printf("\n");
    }
    printf("\n\n");
}

void *producer(void *arg)
{
    char *name = (char *)arg;
    producer_type p_type;

    if (strcmp(name, "driver") == 0)
        p_type = DRIVER;
    else if (strcmp(name, "military") == 0)
        p_type = MILITARY;
    else if (strcmp(name, "material") == 0)
        p_type = MATERIAL;
    else
        return NULL;

    while (!done)
    {

        int weight, range, col_index;
        sem_t *semaphore;

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

        if (convois_processed >= NUM_CONVOIS)
        {
            done = true;

            break;
        }

        sem_wait(semaphore);
        put(weight, range, col_index);

        print_buffer();
        printf("Exiting %s producer\n", name);
    }

    // Release the appropriate semaphore
    sem_post(&sem_driver);
    sem_post(&sem_military);
    sem_post(&sem_material);
    return NULL;
}

int main()
{
    pthread_mutex_init(&mutex_convoi_id, NULL);
    pthread_mutex_init(&mutex_convois_processed, NULL);
    pthread_mutex_init(&mutex_buffer, NULL);
    pthread_mutex_init(&mutex_consumers_done, NULL);

    sem_t *producer_sems[NUM_PRODUCERS] = {&sem_driver, &sem_military, &sem_material};
    sem_t *consumer_sems[NUM_CONSUMERS] = {&sem_plane, &sem_truck, &sem_boat};

    const char *producer_names[] = {"driver", "military", "material"};
    const char *consumer_names[] = {"plane", "truck", "boat"};

    pthread_t producer_threads[NUM_PRODUCERS], consumer_threads[NUM_CONSUMERS];

    // Initialize semaphores and create producer and consumer threads
    for (int i = 0; i < NUM_PRODUCERS; ++i)
    {
        if (sem_init(producer_sems[i], 0, 6) != 0)
        {
            printf("Error initializing semaphore for %s producer\n", producer_names[i]);
            exit(EXIT_FAILURE);
        }

        pthread_create(&producer_threads[i], NULL, producer, (void *)producer_names[i]);
    }
    // Initialize semaphores and create producer and consumer threads
    for (int i = 0; i < NUM_CONSUMERS; ++i)
    {

        if (sem_init(consumer_sems[i], 0, 0) != 0)
        {
            printf("Error initializing semaphore for %s consumer\n", consumer_names[i]);
            exit(EXIT_FAILURE);
        }
        pthread_create(&consumer_threads[i], NULL, consumer, (void *)consumer_names[i]);
    }

    // Join producer threads
    for (int i = 0; i < NUM_PRODUCERS; ++i)
    {
        pthread_join(producer_threads[i], NULL);
    }
    // Join consumer threads
    for (int i = 0; i < NUM_CONSUMERS; ++i)
    {
        pthread_join(consumer_threads[i], NULL);
    }

    // Destroy semaphores producer
    for (int i = 0; i < NUM_PRODUCERS; ++i)
    {
        sem_destroy(producer_sems[i]);
    }
    // Destroy semaphores consumer
    for (int i = 0; i < NUM_CONSUMERS; ++i)
    {
        sem_destroy(consumer_sems[i]);
    }

    pthread_mutex_destroy(&mutex_convoi_id);
    pthread_mutex_destroy(&mutex_convois_processed);
    pthread_mutex_destroy(&mutex_buffer);
    pthread_mutex_destroy(&mutex_consumers_done);

    printf("\n#############################################################################\nAll producer threads have exited\n");

    return 0;
}
