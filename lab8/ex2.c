#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t filled;

void *Produce();
void *Collect();

int main()
{
    pthread_t tid[2];

    // Create semaphore
    sem_init(&filled, 0, 0);

    // Create 2 threads
    pthread_create(&tid[0], NULL, Produce, NULL);
    pthread_create(&tid[1], NULL, Collect, NULL);

    // Stop after 20 seconds
    sleep(20);
    exit(0);
}

// Produces item every 2 seconds
void *Produce()
{
    while (1)
    {
        sleep(2);
        printf("Produced A\n");

        // unlock other thread
        sem_post(&filled);
    }
}

// Collects only 2 items
void *Collect()
{
    while (1)
    {
        // wait for 2 items
        sem_wait(&filled);
        sem_wait(&filled);

        printf("Collected AA\n");
    }
}