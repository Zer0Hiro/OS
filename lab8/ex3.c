#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t mA, mB, mC, mutex;

void* printA();
void* printB();
void* printC();

int main()
{
    pthread_t tid[3];

    // Create semaphore
    sem_init(&mA, 0, 1);
    sem_init(&mB, 0, 0);
    sem_init(&mC, 0, 0);
    sem_init(&mutex, 0, 1);

    // Create threads
    pthread_create(&tid[0], NULL, printA, NULL);
    pthread_create(&tid[1], NULL, printB, NULL);
    pthread_create(&tid[2], NULL, printC, NULL);

    // Stop after 5 seconds
    sleep(5);
    exit(0);
}

void* printA()
{
    while (1)
    {
        // wait for mutex + A
        sem_wait(&mA);
        sem_wait(&mutex);

        printf("A\n");

        // Unlock B
        sem_post(&mB);
        sem_post(&mutex);
    }
}

void* printB()
{
    while (1)
    {
        // wait for mutex + B
        sem_wait(&mB);
        sem_wait(&mutex);

        printf("B\n");

        // Unlock C twice
        sem_post(&mC);
        sem_post(&mC);

        sem_post(&mutex);
    }
}

void* printC()
{
    int temp = 0;
    while (1)
    {
        // wait for mutex + C
        sem_wait(&mC);
        sem_wait(&mutex);

        printf("C\n");

        // continue after 2 prints
        temp++;
        if (temp == 2)
        {
            sem_post(&mA);
            temp = 0;
        }
        sem_post(&mutex);
    }
}