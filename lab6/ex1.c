#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void* f(void* pid);

int curId = 0;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("%s needs 2 arguments\n", argv[0]);
        return 1;
    }

    int amount = atoi(argv[1]);
    pthread_t tid[amount];
    int thread_ids[amount];

    // Create threads
    for (int i = 0; i < amount; i++)
    {
        thread_ids[i] = i + 1;
        pthread_create(&tid[i], NULL, f, &thread_ids[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < amount; i++)
    {
        pthread_join(tid[i], NULL);
    }
    return 0;
}

void* f(void* pid)
{
    int mynum = *(int*)pid;
    struct timeval t0, t1;
    double elapsedTime;

    while (1)
    {
        // Get t0 when thread started
        gettimeofday(&t0, NULL);
        curId = mynum;

        while (mynum == curId) gettimeofday(&t1, NULL);

        // Context switch happened
        elapsedTime = (t1.tv_sec - t0.tv_sec) * 1000.0;
        elapsedTime += (t1.tv_usec - t0.tv_usec) / 1000.0;
        printf("Time slice for thread %d = %.6f ms.\n", mynum, elapsedTime);
    }

    return NULL;
}