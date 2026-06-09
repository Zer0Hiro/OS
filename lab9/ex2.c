#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

sem_t full;       // Taxi is full
sem_t freeSpace;  // Shere is still free space for another student
sem_t arrived;    // Arrive to braude
sem_t emptyTaxi;  // Student left taxi

void* passenger(void* arg);
void* TAXI(void* arg);

int main(int argc, char* argv[])
{
    pthread_t tTaxi;
    pthread_t tArr[N];
    int stdns[N];

    // Init semaphores
    sem_init(&full, 0, 0);
    sem_init(&freeSpace, 0, 4);
    sem_init(&arrived, 0, 0);
    sem_init(&emptyTaxi, 0, 0);  // 0 all students left

    // Create taxi thread
    pthread_create(&tTaxi, NULL, TAXI, NULL);

    // Create passenger threads
    for (int i = 0; i < N; i++)
    {
        stdns[i] = i + 1;
        pthread_create(&tArr[i], NULL, passenger, &stdns[i]);
    }
    sleep(60);
    return 0;
}

// Updates status of passenger
void* passenger(void* arg)
{
    int id = *((int*)arg);

    printf("Student #%d start\n", id);

    // Check if there is still free space in Taxi
    sem_wait(&freeSpace);

    printf("Student #%d is in the TAXI\n", id);

    // Take one seat
    sem_post(&full);

    // Wait until arrive to Braude
    sem_wait(&arrived);

    printf("Student #%d arrived. Out of TAXI\n", id);

    // Leave taxi
    sem_post(&emptyTaxi);
}

// Looped taxi that runs from start point to end point and vice versa
void* TAXI(void* arg)
{
    while (1)
    {
        // Wait for taxi to fill full
        sem_wait(&full);
        sem_wait(&full);
        sem_wait(&full);
        sem_wait(&full);

        printf("Taxi Left Station\n");

        // Drive
        sleep(5);

        printf("Taxi Arrived\n");

        // Tell 4 students they can leave
        sem_post(&arrived);
        sem_post(&arrived);
        sem_post(&arrived);
        sem_post(&arrived);

        // Wait for students to leave
        sem_wait(&emptyTaxi);
        sem_wait(&emptyTaxi);
        sem_wait(&emptyTaxi);
        sem_wait(&emptyTaxi);

        // Drive back
        sleep(5);

        // Allow to new students to enter
        sem_post(&freeSpace);
        sem_post(&freeSpace);
        sem_post(&freeSpace);
        sem_post(&freeSpace);
    }
}