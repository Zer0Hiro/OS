#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define LEFT \
    (i, N)(i + N - 1) % N  // Macro defining the number of the left neighbor
#define RIGHT \
    (i, N)(i + 1) % N  // Macro defining the number of the right neighbor
#define THINKING 0     // Philosopher is thinking
#define HUNGRY 1       // Philosopher is hungry (waiting for chopsticks)
#define EATING 2       // Philosopher is eating
#define MAX_TIME 5  // Maximum time allowed to spend on eating and/or thinking

int N;                 // Number of philosophers
sem_t* mutex = NULL;   // Pointer to a binary semaphore
sem_t* eaters = NULL;  // Array of semaphores for philosophers
int* state = NULL;  // Array of integers to determine philosopher state (eating,
                    // hungry, thinking)

// Eating process
void eat(int i)
{
    int time = rand() % MAX_TIME;
    printf("Philosopher #%d is eating...\n", i + 1);
    sleep(time);
    printf("Philosopher #%d stopped eating...\n", i + 1);
}

// Thinking process
void think(int i)
{
    int time = rand() % MAX_TIME;
    printf("Philosopher #%d is thinking...\n", i + 1);
    sleep(time);
}

// Check if a hungry philosopher can start eating
void test(int i)
{
    if (state[i] == HUNGRY && state[i - 1] != EATING && state[i + 1] != EATING)
    {
        state[i] = EATING;
        sem_post(&eaters[i]);
    }
}

// Philosopher tries to take a chopstick
void take_sticks(int i)
{
    sem_wait(mutex);
    // Philosopher is hungry
    state[i] = HUNGRY;
    // Try to take 2 chopsticks
    test(i);
    sem_post(mutex);
    sem_wait(&eaters[i]);
}

// Philosopher returns a chopstick
void put_sticks(int i)
{
    sem_wait(mutex);
    // Philosopher finished
    state[i] = THINKING;
    // Check if philosophers hungry from left and right
    test(i - 1);
    test(i + 1);
    sem_post(mutex);
}

// This code is executed by each thread describing a philosopher's behavior
void* philosopher(void* arg)
{
    int i = *((int*)arg);
    while (1 == 1)
    {
        think(i);
        take_sticks(i);
        eat(i);
        put_sticks(i);
    }
}

int main(int argc, char* argv[])
{
    void* result;
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <NUMBER_OF_PHILOSOPHERS>\n", argv[0]);
        return 1;
    }
    // Preparation...
    N = atoi(argv[1]);  // Convert the amount of philosophers from a string to
                        // an integer
    if (N <= 1)
    {
        fprintf(stderr, "Error by transformation of the argument...\n");
        return 2;
    }  // Not less than 2 philosophers

    mutex = (sem_t*)malloc(sizeof(sem_t));
    if (sem_init(mutex, 0, 1) != 0)
    {
        // Create a mutex for separation at the entrance to the critical section
        fprintf(stderr, "Error by creating semaphore...\n");
        return 3;
    }

    // Create and init semaphores of eaters
    eaters = (sem_t*)malloc(sizeof(sem_t) * N);
    // Allocate memory states of professors
    state = (int*)malloc(sizeof(int) * N);

    memset(state, 0, N);

    srand(time(NULL));
    pthread_t* philosophers = (pthread_t*)malloc(N * sizeof(pthread_t));

    int i;
    for (i = 0; i < N; i++)
    {
        if (sem_init(&eaters[i], 0, 0) != 0)
        {  // Initialize philosophers' semaphores
            fprintf(stderr, "Error by creating semaphore...\n");
            return 3;
        }
    }

    for (i = 0; i < N; i++)
    {
        if (pthread_create(&philosophers[i], NULL, philosopher, (void*)&i) != 0)
        {
            fprintf(stderr, "Error by creating thread\n");
            return 2;
        }
        usleep(100000);
    }

    for (i = 0; i < N; i++)
    {
        if (pthread_join(philosophers[i], &result) != 0)
        {
            fprintf(stderr, "Error by joining thread\n");
            return 3;
        }
        return 0;
    }
}