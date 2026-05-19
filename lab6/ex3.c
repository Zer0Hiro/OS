#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#define N 3

void fillMatrix(int size, int arr[][N]);
void printMatrix(int size, int arr[][N]);
void* sum(void* arg);

int arrOne[N][N];
int arrTwo[N][N];
int arrFin[N][N];
int arr[N];

int main()
{
    pthread_t tid[N];
    setbuf(stdout, NULL);
    // Get all numbers from matrix
    fillMatrix(N, arrOne);
    fillMatrix(N, arrTwo);

    // Print matrixes
    printMatrix(N, arrOne);
    printMatrix(N, arrTwo);

    // Create threads
    for (int i = 0; i < N; i++)
    {
        arr[i] = i;
        pthread_create(&tid[i], NULL, sum, &arr[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < N; i++)
    {
        pthread_join(tid[i], NULL);
    }
    printMatrix(N, arrFin);
    return 0;
}

// Fills matrix with numbers from stdin
void fillMatrix(int size, int arr[N][N])
{
    for (int i = 0; i < size; i++)
    {
        printf("Enter %d line of matrix %dx%d: ", i + 1, size, size);
        for (int j = 0; j < size; j++)
        {
            scanf("%d", &arr[i][j]);
        }
    }
    printf("--------\n");
}

// Prints the whole matrix of
void printMatrix(int size, int arr[][N])
{
    for (int i = 0; i < size; i++)
    {
        printf("[");
        for (int j = 0; j < size - 1; j++)
        {
            printf("%3d ", arr[i][j]);
        }
        printf("%3d]\n", arr[i][N - 1]);
    }
    printf("----------\n");
}

// Summs each element of the row with series: A[i][k] * B[k][j]
void* sum(void* arg)
{
    int pos = *(int*)arg;

    for (int j = 0; j < N; j++)
    {
        arrFin[pos][j] = 0;
        for (int k = 0; k < N; k++)
        {
            arrFin[pos][j] += arrOne[pos][k] * arrTwo[k][j];
        }
    }
}