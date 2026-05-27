#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *pA();
void *pB();

sem_t mA, mB;

int main()
{
    pthread_t tid[2];
    sem_init(&mA, 0, 1);
    sem_init(&mB, 0, 0);

    // Create threads
    pthread_create(&tid[0], NULL, pA, NULL);
    pthread_create(&tid[1], NULL, pB, NULL);

    pthread_join(tid[0], NULL);
    exit(0);
}

void *pA()
{
    while (1)
    {
        /* Semaphore sync 
        sem_wait: -1 to semaphore, if already 0 blocks thread

        sem_post: +1 to semaphore
        */
        sem_wait(&mA);
        printf("A");
        sem_post(&mB);
    }
}

void *pB()
{
    while (1)
    {
        sem_wait(&mB);
        printf("B");
        sem_post(&mA);
    }
}