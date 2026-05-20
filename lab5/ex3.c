#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 10

void *search(void *pos);

int arr[N];
int num;
pthread_t tid[2];
int flag = 0;

int main()
{
    // random seed for rand()
    srand(time(NULL));

    // Fill arr with rand numbers 0-100
    for (int i = 0; i < N; i++)
    {
        arr[i] = rand() % 101;
        printf("%d ", arr[i]);
    }

    // Get number
    printf("\ninput: ");
    scanf("%d", &num);

    // Create threads
    int posEven = 0;
    if (pthread_create(&tid[0], NULL, search, &posEven) != 0)
    {
        perror("Thread failed");
        exit(1);
    }

    int posOdd = 1;
    if (pthread_create(&tid[1], NULL, search, &posOdd) != 0)
    {
        perror("Thread failed");
        exit(1);
    }

    // Wait for threads
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    if (flag)
    {
        printf("%d is part of the matrix\n", num);
        return 0;
    }
    printf("%d not in matrix\n", num);
    return 1;
}

void *search(void *pos)
{
    for (int i = *(int *)pos; i < N; i += 2)
    {
        // Check if other thread found
        if (flag)
            return 0;

        if (arr[i] == num)
        {
            flag = 1;
            return 0;
        }
    }
}