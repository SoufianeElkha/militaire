#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

// Sem for producer
sem_t sem_driver;
sem_t sem_military;
sem_t sem_material;

// Sem for consumer
sem_t sem_plane;
sem_t sem_truck;
sem_t sem_boat;

int convoi_id = 0;
pthread_mutex_t mutex_convoi_id;
pthread_mutex_t mutex_delete;
int buffer[6][5] = {0}; // Initializing all values to 0

void put(int weight, int range, int col_index)
{
    int timeToWait = (rand() % 2 == 0) ? 8 : 12;
    printf("time to wait : %d\n", timeToWait);
    usleep(1000 * timeToWait);
    int sem_value;

    for (int i = 0; i < 6; i++)
    {
        if (buffer[i][0] != 0 && buffer[i][1] != 0 && buffer[i][2] != 0)
        {
            continue; // AGGIUNTO: controllo per evitare di riempire una riga già piena
        }

        if (buffer[i][col_index] == 0)
        {
            int random_num = rand() % (2 * range + 1) - range; // Générer un nombre aléatoire entre -range et range
            buffer[i][col_index] = weight + random_num;        // Ajouter le nombre aléatoire au poids
            if (buffer[i][0] != 0 && buffer[i][1] != 0 && buffer[i][2] != 0)
            {
                buffer[i][3] = buffer[i][0] + buffer[i][1] + buffer[i][2];
                pthread_mutex_lock(&mutex_convoi_id);
                buffer[i][4] = ++convoi_id;
                pthread_mutex_unlock(&mutex_convoi_id);
                if (buffer[i][3] < 10001)
                {
                    sem_post(&sem_plane);
                    sem_getvalue(&sem_plane, &sem_value);
                    printf("Plane sem: %d\n", sem_value);
                }
                else if (buffer[i][3] > 10001 && buffer[i][3] < 30001)
                {
                    sem_post(&sem_truck);
                    sem_getvalue(&sem_truck, &sem_value);
                    printf("Truck sem: %d\n", sem_value);
                }
                else if (buffer[i][3] > 30001)
                {
                    sem_post(&sem_boat);
                    sem_getvalue(&sem_boat, &sem_value);
                    printf("Boat sem: %d\n", sem_value);
                }
            }
            break;
        }
    }
}

/*                pthread_mutex_lock(&mutex_delete);
                for (int i = 0; i < 6; i++)
                {
                    buffer[tmp_i][i] = 0;
                    printf("Tmp_i_2 %d", tmp_i);
                }
                sem_post(&sem_driver);
                sem_post(&sem_military);
                sem_post(&sem_material);

                pthread_mutex_unlock(&mutex_delete);*/
void get(void *arg)
{
    char *name = (char *)arg;
    int convoi_nb = 300;
    int tmp_i;

    pthread_mutex_lock(&mutex_delete); // Spostato: bloccare il mutex prima di leggere dal buffer

    if (strcmp(name, "plane") == 0)
    {
        for (int i = 0; i < 6; i++)
        {
            if (buffer[i][3] < 10001 && buffer[i][4] < convoi_nb)
            {
                convoi_nb = buffer[i][4];
                tmp_i = i;
            }
        }
    }
    else if (strcmp(name, "truck") == 0)
    {
        for (int i = 0; i < 6; i++)
        {
            if (buffer[i][3] > 10001 && buffer[i][3] < 30001 && buffer[i][4] < convoi_nb)
            {
                convoi_nb = buffer[i][4];
                tmp_i = i;
            }
        }
    }
    else if (strcmp(name, "boat") == 0)
    {
        for (int i = 0; i < 6; i++)
        {
            if (buffer[i][3] > 30001 && buffer[i][4] < convoi_nb)
            {
                convoi_nb = buffer[i][4];
                tmp_i = i;
            }
        }
    }

    if (convoi_nb != 300) // Aggiunto: controllo per evitare la cancellazione se non trovato un convoglio appropriato
    {
        for (int i = 0; i < 5; i++)
        {
            buffer[tmp_i][i] = 0;
        }
        sem_post(&sem_driver);
        sem_post(&sem_military);
        sem_post(&sem_material);
    }

    pthread_mutex_unlock(&mutex_delete); // Spostato: sbloccare il mutex dopo aver letto e aggiornato il buffer
}

void *consumer(void *arg)
{
    char *name = (char *)arg;

    while (true)
    {
        if (strcmp(name, "plane") == 0)
        {
            sem_wait(&sem_plane);
            get("plane");
            printf("Plane is going\n");
        }
        else if (strcmp(name, "truck") == 0)
        {
            sem_wait(&sem_truck);
            get("truck");
            printf("Truck is going\n");
        }
        else if (strcmp(name, "boat") == 0)
        {
            sem_wait(&sem_boat);
            get("boat");
            printf("Boat is going\n");
        }
    }
}

void *producer(void *arg)
{
    char *name = (char *)arg;

    // Attente du sémaphore approprié
    while (true)
    {
        if (strcmp(name, "driver") == 0)
        {
            sem_wait(&sem_driver);
            put(80, 20, 0);
        }
        else if (strcmp(name, "military") == 0)
        {
            sem_wait(&sem_military);
            put(8000, 7000, 1);
        }
        else if (strcmp(name, "material") == 0)
        {
            sem_wait(&sem_material);
            put(25000, 24500, 2);
        }

        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                printf("%d ", buffer[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("\n");
        printf("\n");
    }

    printf("Starting %s producer\n", name);
    // Code de production ici
    printf("Exiting %s producer\n", name);

    // Libération du sémaphore approprié
    if (strcmp(name, "driver") == 0)
    {
        sem_post(&sem_driver);
    }
    else if (strcmp(name, "military") == 0)
    {
        sem_post(&sem_military);
    }
    else if (strcmp(name, "material") == 0)
    {
        sem_post(&sem_material);
    }

    pthread_exit(NULL);
}

int main()
{

    pthread_mutex_init(&mutex_convoi_id, NULL);
    pthread_mutex_init(&mutex_delete, NULL);

    // Loop to print all values in the buffer
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d ", buffer[i][j]);
        }
        printf("\n");
    }

    // Initialisation des sémaphores producer
    if (sem_init(&sem_driver, 0, 6) != 0)
    {
        printf("Error initializing semaphore for driver producer\n");
        exit(EXIT_FAILURE);
    }

    if (sem_init(&sem_military, 0, 6) != 0)
    {
        printf("Error initializing semaphore for military producer\n");
        exit(EXIT_FAILURE);
    }

    if (sem_init(&sem_material, 0, 6) != 0)
    {
        printf("Error initializing semaphore for material producer\n");
        exit(EXIT_FAILURE);
    }

    // Initialisation des sémaphores consumer
    if (sem_init(&sem_plane, 0, 0) != 0)
    {
        printf("Error initializing semaphore for driver producer\n");
        exit(EXIT_FAILURE);
    }

    if (sem_init(&sem_truck, 0, 0) != 0)
    {
        printf("Error initializing semaphore for military producer\n");
        exit(EXIT_FAILURE);
    }

    if (sem_init(&sem_boat, 0, 0) != 0)
    {
        printf("Error initializing semaphore for material producer\n");
        exit(EXIT_FAILURE);
    }

    pthread_t prod_driver_thread, prod_military_thread, prod_material_thread, plane_thread, truck_thread, boat_thread;

    // Création des threads producteurs
    pthread_create(&prod_driver_thread, NULL, producer, "driver");
    pthread_create(&prod_military_thread, NULL, producer, "military");
    pthread_create(&prod_material_thread, NULL, producer, "material");

    pthread_create(&plane_thread, NULL, consumer, "plane");
    pthread_create(&truck_thread, NULL, consumer, "truck");
    pthread_create(&boat_thread, NULL, consumer, "boat");

    // Attente de la fin des threads producteurs
    pthread_join(prod_driver_thread, NULL);
    pthread_join(prod_military_thread, NULL);
    pthread_join(prod_material_thread, NULL);

    pthread_join(plane_thread, NULL);
    pthread_join(truck_thread, NULL);
    pthread_join(boat_thread, NULL);

    // Destruction des sémaphores
    sem_destroy(&sem_driver);
    sem_destroy(&sem_military);
    sem_destroy(&sem_material);

    sem_destroy(&sem_plane);
    sem_destroy(&sem_truck);
    sem_destroy(&sem_boat);

    pthread_mutex_destroy(&mutex_convoi_id);
    pthread_mutex_destroy(&mutex_delete);

    printf("All producer threads have exited\n");

    return 0;
}
