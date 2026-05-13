#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUFFER 1024
#define ARG_SIZE 10

int enoughArgs(int arguments, int size);

int main()
{
    char input[BUFFER];
    char* args[ARG_SIZE];
    int pid;
    int size;

    printf("Entering Catalog Shell...\n");

    while (1)
    {
        printf("Catalog$** ");
        if (fgets(input, BUFFER, stdin) == NULL)
        {
            perror("fgets failed!\n");
            exit(1);
        }

        // Stop condition
        if (strcmp("Esc\n", input) == 0) break;

        input[strcspn(input, "\n")] = '\0';

        // Tokenize arguments
        int i = 0;
        char* token = strtok(input, " ");

        while (token != NULL && i < ARG_SIZE - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;
        size = i;

        // Run command
        pid = fork();
        if (pid < 0)
        {
            perror("Fork failed");
            exit(1);
        }

        // Child
        if (pid == 0)
        {
            // find command
            if (strcmp(args[0], "find") == 0)
            {
                if (!enoughArgs(size, 3)) exit(1);
                execvp("grep", args);
            }
            // newcat command
            if (strcmp(args[0], "newcat") == 0)
            {
                if (!enoughArgs(size, 2)) exit(1);
                execvp("mkdir", args);
            }
            // ls command
            if (strcmp(args[0], "ls") == 0)
            {
                if (!enoughArgs(size, 2)) exit(1);
                execvp(args[0], args);
            }
            // run command
            if (strcmp(args[0], "run") == 0)
            {
                if (!enoughArgs(size, 3)) exit(1);
                execvp(args[1], args + 1);
            }

            // fails
            printf("Exec failed, command does not exist\n");
            exit(1);
        }
        else
        {
            wait(NULL);
        }
    }
    printf("Returning to LibShell...\n");
    return 0;
}

// Check if amount of arguments smaller than we demand
int enoughArgs(int arguments, int size)
{
    if (arguments < size)
    {
        printf("Missing parameters!!!\n");
        return 0;
    }
    if (arguments > size)
    {
        printf("Not Supported!!!\n");
        return 0;
    }
    return 1;
}