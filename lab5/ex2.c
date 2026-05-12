#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* count();
void* logs();

unsigned long int counter = 0;

int main()
{
    pthread_t tid[2];

    // Create threads
    if (pthread_create(&tid[0], NULL, count, NULL) != 0)
    {
        perror("Thread th1 failed");
        exit(1);
    }

    if (pthread_create(&tid[1], NULL, logs, NULL) != 0)
    {
        perror("Thread th2 failed");
        exit(1);
    }

    // Wait for logs thread
    pthread_join(tid[1], NULL);
    return 0;
}

void* count()
{
    while (1) counter++;
}

// Prints every 100ms counter
void* logs()
{
    for (int i = 0; i < 10; i++)
    {
        printf("%lu\n", counter);
        usleep(100000);
    }
}