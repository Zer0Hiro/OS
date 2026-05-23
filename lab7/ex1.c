#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 10

int main()
{
    int c1, c2, t, i;
    c2 = 1;
    c1 = 0;

    for (i = 1; i < N; i++)
    {
        switch (fork())
        {
            case -1:
                printf("Error creating process\n");
                return -1;

            // Child process
            case 0:
                // Calc new number
                t = c2;
                c2 = c1 + t;
                c1 = t;
                break;

            // Parent process
            default:
                i = N;
                break;
        }
    }
    // Print current number
    printf("%d ", c2);

    // Wait for child process
    wait(NULL);
    return 0;
}
