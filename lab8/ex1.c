#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sem_t mA, mB;

void* create_message(void* str)
{
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        sem_wait(&mB);
        printf("I've wrote a message #%d. %s\n", i + 1, (char*)str);
        sem_post(&mA);
    }
}

int main()
{
    pthread_t thread;

    // create new semaphores
    sem_init(&mA, 0, 0);
    sem_init(&mB, 0, 1);

    int i = 0;
    pthread_create(&thread, NULL, create_message, (void*)"Thread A");
    for (i = 0; i < 10; i++)
    {
        // lock thread to run only once
        sem_wait(&mA);
        printf("The message #%d was printed. Thread B \n", i + 1);
        printf("\n");
        // unlock second thread
        sem_post(&mB);
    }
    pthread_join(thread, NULL);
    exit(0);
}
