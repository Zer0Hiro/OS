#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *count();
void *logs();

unsigned long int counter = 0;

int main()
{
    pthread_t tid[2];

    pthread_create(&tid[0], NULL, count, NULL);
    pthread_create(&tid[1], NULL, logs, NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
}

void *count()
{
    while (1)
        counter++;
}

void *logs()
{
    while (1)
    {
        printf("%lu", counter);
        usleep(100000);
    }
}