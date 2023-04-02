#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NB_SLOTS_IN_DEPOT 6
#define NB_CONVOYS 200
#define AIR 0
#define ROAD 1
#define RIVER 2
#define DRIVER 0
#define CARGO 1
#define TROUP 2
#define LIGHT 0
#define MEDIUM 1
#define HEAVY 2

unsigned convoy_weights[NB_SLOTS_IN_DEPOT][3];
int delivered_convoys[3] = {0, 0, 0};
int produced_convoys = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hangar_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t hangar_not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t convoy_available[3] = {PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER};

unsigned random_weight(unsigned min, unsigned max)
{
    return min + (rand() % (max - min + 1));
}

void put(unsigned type, unsigned weight)
{
    pthread_mutex_lock(&mutex);

    while (produced_convoys >= NB_CONVOYS)
    {
        pthread_cond_wait(&hangar_not_full, &mutex);
    }

    int index = 0;
    while (convoy_weights[index][type] != 0 && index < NB_SLOTS_IN_DEPOT-1)

    {
        index++;
    }
    convoy_weights[index][type] = weight;

    produced_convoys++;
    pthread_cond_broadcast(&hangar_not_empty);

    pthread_mutex_unlock(&mutex);
}




unsigned get(int weight_category)
{
    unsigned total_weight = 0;

    pthread_mutex_lock(&mutex);

    while (delivered_convoys[0] + delivered_convoys[1] + delivered_convoys[2] >= NB_CONVOYS)
    {
        pthread_cond_wait(&convoy_available[weight_category], &mutex);
    }

    int index = 0;
    // Find a suitable index with a non-zero weight for DRIVER and TROUP
    while ((convoy_weights[index][DRIVER] == 0 || convoy_weights[index][TROUP] == 0) && index < NB_SLOTS_IN_DEPOT-1) {
        index++;
    }

    total_weight = convoy_weights[index][DRIVER] + convoy_weights[index][CARGO] + convoy_weights[index][TROUP];
    convoy_weights[index][DRIVER] = 0;
    convoy_weights[index][CARGO] = 0;
    convoy_weights[index][TROUP] = 0;

    delivered_convoys[weight_category]++;
    pthread_cond_broadcast(&hangar_not_full);

    pthread_mutex_unlock(&mutex);

    return total_weight;
}




void *producer(void *arg)
{
    unsigned type = (unsigned)(size_t)arg;

    for (int i = 0; i < NB_CONVOYS / 3; ++i)
    {
        unsigned weight;
        if (type == DRIVER)
        {
            weight = random_weight(80 - 20, 80 + 20);
        }
        else if (type == CARGO)
        {
            weight = random_weight(25000 - 24500, 25000 + 24500);
        }
        else
        {
            weight = random_weight(8000 - 7000, 8000 + 7000);
        }

        put(type, weight);
        usleep(random_weight(10 - 2, 10 + 2) * 1000);
    }

    return NULL;
}

void *consumer(void *arg)
{
    int weight_category = (int)(size_t)arg;
    int destination;

    if (weight_category == LIGHT)
    {
        destination = AIR;
    }
    else if (weight_category == MEDIUM)
    {
        destination = ROAD;
    }
    else
    {
        destination = RIVER;
    }

    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (delivered_convoys[0] + delivered_convoys[1] + delivered_convoys[2] >= NB_CONVOYS)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        unsigned total_weight = get(weight_category);

        if (destination == AIR)
        {
            printf("Convoi %d (Avion): %u kg\n", delivered_convoys[weight_category], total_weight);
        }
        else if (destination == ROAD)
        {
            printf("Convoi %d (Camion): %u kg\n", delivered_convoys[weight_category], total_weight);
        }
        else
        {
            printf("Convoi %d (Bateau): %u kg\n", delivered_convoys[weight_category], total_weight);
        }
    }

    return NULL;
}

int main()
{
    srand(time(NULL));
    pthread_t producers[3];
    pthread_t consumers[3];

    for (int i = 0; i < 3; ++i)
    {
        pthread_create(&producers[i], NULL, producer, (void *)(size_t)i);
        pthread_create(&consumers[i], NULL, consumer, (void *)(size_t)i);
    }

    for (int i = 0; i < 3; ++i)
    {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    printf("Totale consegne: Aéroport: %d, Parking: %d, Port: %d\n", delivered_convoys[AIR], delivered_convoys[ROAD], delivered_convoys[RIVER]);

    return 0;
}
