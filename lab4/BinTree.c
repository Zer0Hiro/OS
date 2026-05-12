#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        perror("Not enough Arguments");
        exit(1);
    }

    int leftPid, rightPid;

    printf("Parent PID: %d\n", getpid());

    for (int i = 0; i < atoi(argv[1]) - 1; i++)
    {
        // Left Nodes
        leftPid = fork();
        if (leftPid == -1)
        {
            perror("fork");
            exit(1);
        }
        // Child process
        if (leftPid == 0)
        {
            printf("Left child: Process PID: %d, Parent PID: %d\n", getpid(),
                   getppid());
            continue;
        }

        // Right Nodes
        rightPid = fork();
        if (rightPid == -1)
        {
            perror("fork");
            exit(1);
        }
        // Child process
        if (rightPid == 0)
        {
            printf("Right child: Process PID: %d, Parent PID: %d\n", getpid(),
                   getppid());
            continue;
        }

        wait(NULL);
        wait(NULL);
        break;
    }
    return 0;
}