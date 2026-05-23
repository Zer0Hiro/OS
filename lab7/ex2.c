#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 220
#define M 284

void* sum(void* args);

pthread_t tid[2];

int main()
{
    int arr[2] = {N, M};
    void *sum1, *sum2;

    // Create threads
    pthread_create(&tid[0], NULL, sum, &arr[0]);
    pthread_create(&tid[1], NULL, sum, &arr[1]);

    // Wait for threads to finish
    pthread_join(tid[0], &sum1);
    pthread_join(tid[1], &sum2);

    if (N == *(int*)sum2 && M == *(int*)sum1)
        printf("Amicable Numbers\n");
    else
        printf("Not Amicable Numbers\n");

    // free memory
    free(sum1);
    free(sum2);
    return 0;
}

void* sum(void* args)
{
    int num = *(int*)args;
    int sum = 1;

    // look for divisors
    for (int i = 2; i <= sqrt(num); i++)
    {
        if (num % i == 0)
        {
            sum += i;
            int temp = num / i;

            if (i != temp) sum += temp;
        }
    }

    // allocate memory for sum
    int* result = malloc(sizeof(int));
    *result = sum;

    return result;
}