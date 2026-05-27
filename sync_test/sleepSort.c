#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// The payload we send to Ohio (the thread arguments)
typedef struct
{
    int gyatt_level;
} Rizzler;

// The function where the thread starts mewing (sleeping)
void *edge_and_burst(void *arg)
{
    Rizzler *rizz = (Rizzler *)arg;
    int val = rizz->gyatt_level;

    // Edging the thread by sleeping.
    // We multiply by 10000 microseconds (10ms) per unit so we
    // don't literally wait a whole year for the Fanum tax to hit.a
    usleep(val * 1000);

    // The thread finally cooked and drops the output
    printf("%d ", val);

    // Being a responsible sigma and preventing memory leaks
    free(rizz);
    return NULL;
}

int main()
{
    // The unsorted NPCs
    int chat_array[] = {6, 2, 9, 1, 5, 100, 921, 812, 8, 3};
    int len = sizeof(chat_array) / sizeof(chat_array[0]);

    printf("Unsorted NPCs: ");
    for (int i = 0; i < len; i++)
    {
        printf("%d ", chat_array[i]);
    }
    printf("\n\n");

    pthread_t sigmas[len];

    // Spawn a thread for each number so they can looksmax independently
    for (int i = 0; i < len; i++)
    {
        Rizzler *rizz = malloc(sizeof(Rizzler));
        rizz->gyatt_level = chat_array[i];

        // Yeet the thread into existence
        pthread_create(&sigmas[i], NULL, edge_and_burst, rizz);
    }

    // The main thread needs to wait so the terminal doesn't get Fanum taxed
    // before the worker threads finish outputting.
    // We sleep the main thread long enough for the biggest number to finish.
    for (int i = 0; i < len; i++)
        pthread_join(sigmas[i], NULL);

    return 0;
}