#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 4  // Number of callers

char* cities[] = {"Carmiel", "Tiberias", "Haifa", "Tzfat"};
int senior_cities[N] = {0};
sem_t cities_sem[N];

sem_t senior;

void Calls(int City1, int City2);
void* Func(void* arg);

int main()
{
    pthread_t tid[N];
    int cityId[N];

    srand(time(NULL));
    // Give senior cities prio
    sem_init(&senior, 0, 1);

    for (int i = 0; i < N; i++)
    {
        // Init semaphore for city
        sem_init(&cities_sem[i], 0, 1);
        senior_cities[2] = 1;
        senior_cities[3] = 1;

        // Init threads
        cityId[i] = i;
        pthread_create(&tid[i], NULL, Func, &cityId[i]);
    }
    sleep(20);
    return 0;
}

// Prints which cities call to each other, and locks them as busy
void Calls(int City1, int City2)
{
    if (senior_cities[City1] && senior_cities[City2])
    {
        sem_wait(&senior);
    }
    // Not Senior Cities
    else
    {
        sem_wait(&senior);
        sem_post(&senior);
    }

    sem_wait(&cities_sem[City1]);

    // Check if city2 is locked
    if (sem_trywait(&cities_sem[City2]) == 0)
    {
        printf("%s started to talk with %s\n", cities[City1], cities[City2]);
        usleep(50000);
        printf("%s finished to talk with %s\n", cities[City1], cities[City2]);

        // unlock cities
        sem_post(&cities_sem[City2]);
        sem_post(&cities_sem[City1]);
    }
    else
    {
        sem_post(&cities_sem[City1]);
    }

    // Update semaphore for senior cities
    if (senior_cities[City1] && senior_cities[City2])
    {
        sem_post(&senior);
    }
}

void* Func(void* arg)
{
    // Get current city id
    int city1 = *(int*)arg;

    while (1)
    {
        int city2 = city1;
        // Get rand city to call
        while (city1 == city2)
        {
            city2 = rand() % N;
        }

        // Try to call
        Calls(city1, city2);

        // Anti Starvation
        usleep(10000);
    }
}