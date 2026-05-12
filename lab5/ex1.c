#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#define N 10 // rows
#define M 30 // cols

void *numbers(void *args);
void *count(void *args);

pthread_t tid[N];
int matrix[N][M];
int sum = 0;

int main()
{
    srand(time(NULL));
    int arr[N];
    int i;
    // Create threads
    for (i = 0; i < N; i++)
    {
        arr[i] = i;
        pthread_create(&tid[i], NULL, numbers, &arr[i]);
    }

    // Wait for threads
    for (i = 0; i < N; i++)
    {
        pthread_join(tid[i], NULL);
    }

    // Print matrix
    for (i = 0; i < N; i++)
    {
        printf("( ");
        for (int j = 0; j < M; j++)
        {
            printf("%03d%s", matrix[i][j], j == M - 1 ? "" : ", ");
        }
        printf(")\n");
    }

    // Threads to count
    for (i = 0; i < N; i++)
    {
        arr[i] = i;
        pthread_create(&tid[i], NULL, count, &arr[i]);
    }

    // Wait for threads
    for (i = 0; i < N; i++)
    {
        pthread_join(tid[i], NULL);
    }

    // Print sum
    printf("\nSum of matrix: %d\n", sum);
}

// Fill matrix with rand numbers 0-100
void *numbers(void *args)
{
    int n = *(int *)args;
    for (int i = 0; i < M; i++)
    {
        matrix[n][i] = rand() % 101;
    }
}

void *count(void *args)
{
    int n = *(int *)args;
    for (int i = 0; i < M; i++)
    {
        sum += matrix[n][i];
    }
}
