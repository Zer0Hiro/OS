#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void* summary(void* args);
void* counter(void* args);

// Struct for counters/summary
typedef struct
{
    int tid;
    int size;
    long int* counters;
} CounterArgs;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("%s needs 2 arguments\n", argv[0]);
        return 1;
    }

    // Get amount of threads
    int size = atoi(argv[1]);

    long int counters[size];
    pthread_t tid[size + 1];
    CounterArgs count_args[size];

    // Fill with zeros
    for (int i = 0; i < size; i++) counters[i] = 0;

    // Create counting threads
    for (int i = 0; i < size; i++)
    {
        count_args[i].tid = i;
        count_args[i].size = size;
        count_args[i].counters = counters;

        pthread_create(&tid[i], NULL, counter, &count_args[i]);
    }
    pthread_create(&tid[size], NULL, summary, &count_args[0]);

    sleep(20);
    exit(0);
}

// Infinitely growing counter
void* counter(void* args)
{
    // get pointer of struct
    CounterArgs* arguments = (CounterArgs*)args;

    int id = arguments->tid;
    long int* counter = arguments->counters;

    while (1)
    {
        counter[id]++;
    }
}

// Summerize all counters every 2 seconds
void* summary(void* args)
{
    // get pointer of struct
    CounterArgs* arguments = (CounterArgs*)args;

    int size = arguments->size;
    long int* counters = arguments->counters;

    while (1)
    {
        // Print all counters
        for (int i = 0; i < size; i++)
        {
            printf("Counter of Thread %d = %ld\n", i + 1, counters[i]);
        }
        sleep(2);
    }
}