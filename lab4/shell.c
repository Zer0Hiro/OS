#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUFFER 1024
#define ARG_SIZE 10

int main(int argc, char* argv[])
{
    char input[BUFFER];
    char* args[ARG_SIZE];
    int pid;

    // Disable buffer
    setbuf(stdout, NULL);

    while (1)
    {
        printf("$ ");
        // Get arguments
        if (fgets(input, BUFFER, stdin) != NULL)
        {
            input[strcspn(input, "\n")] = '\0';
            // Check if exit
            if (strcmp("q", input) == 0) break;
        };

        // Tokenize arguments
        int i = 0;
        char* token = strtok(input, " ");  

        while (token != NULL && i < ARG_SIZE - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " ");  // Next argument
        }
        args[i] = NULL;

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
            execvp(args[0], args);

            // if fails
            printf("ERROR\n");
            exit(1);
        }
        else
        {
            wait(NULL);
        }
    }
    printf("END\n");
    return 0;
}