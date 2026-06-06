#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define K 5
#define N 10

sem_t mutex, empty, freeSpace;

int stk[N];
int idx;

void stkPush(int num);
void* tFunc(void* p);
int stkPop();

int main(int argc, char* argv[])
{
    pthread_t tArr[K];
    int ans[K];

    // Init semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, 0); // Is that empty
    sem_init(&freeSpace, 0, N); // Is there a free space

    // Create K threads
    for (int i = 0; i < K; i++)
    {
        ans[i] = pthread_create(&tArr[i], NULL, tFunc, NULL);
    }
    sleep(10);
    return 0;
}

// Push new element to stack
void stkPush(int num)
{
    // lock stk for this func
    sem_wait(&freeSpace);
    sem_wait(&mutex);

    printf("PUSH %d\n", num);
    stk[idx++] = num;
    
    sem_post(&empty);
    // unlock stk
    sem_post(&mutex);
}

// Pop last element from the stack
int stkPop()
{
    sem_wait(&empty);
    sem_wait(&mutex);

    printf("POP %d\n", stk[idx-1]);
    idx--;

    sem_post(&freeSpace);
    sem_post(&mutex);
}

void* tFunc(void* p)
{
    int num;
    while (1)
    {
        num = rand() % 100;
        if (rand() % 2)
        {
            stkPush(num);
        }
        else
        {
            stkPop();
        }
        sleep(1);
    }
}
