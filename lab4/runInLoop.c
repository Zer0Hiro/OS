#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    // arg check
    if (argc < 3)
    {
        perror("Not enough Arguments");
        exit(1);
    }

    int pid;
    int amount = atoi(argv[2]);
    // Loop same process amount of times
    for (int i = 0; i < amount; i++)
    {
        pid = fork();
        // Fork failed
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }

        // Child process
        if (pid == 0)
        {
            execv(argv[1], NULL);

            // if fails
            perror("execv failed");
            exit(1);
        }
        // Parent process
        else
        {
            wait(NULL);
        }
    }
    printf("END\n");
    return 0;
}