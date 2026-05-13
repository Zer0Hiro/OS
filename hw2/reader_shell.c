#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>  // For wait()
#include <unistd.h>    // For fork(), exec(), pid_t
#define SIZE 500

// Argc - counts the amount of words written in the prompt
// Argv - an array with said words
int main(int argc, char* argv[])
{
    printf("Entering Reader Shell... \n");
    // argv[0] - Program name
    // Checks to see if the user provided enough arguments (exactly 1)
    if (argc > 1)
    {
        printf("\nError! This program does not accept additional arguments.\n");
        exit(1);
    }

    char input[SIZE] = "";

    while (1)
    {
        // Gets user input
        printf("Reader$** ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            perror("fgets failed!\n");
            exit(1);
        }

        // Gets rid of the '\n' that is created by pressing enter.
        input[strcspn(input, "\n")] = '\0';

        // Stop condition
        if (strcmp(input, "Esc") == 0) break;

        // Initializes a new process and checks for errors
        int pid = fork();

        if (pid < 0)
        {
            fprintf(stderr, "Fork failed.\n");
            exit(1);
        }

        // Child process
        if (pid == 0)
        {
            if (execlp(input, input, NULL) == -1) printf("Not Supported\n");
            exit(1);  // Kills the child process if fork() fails.
        }

        // Parent process isn't supposed to do anything
        else
            wait(NULL);
    }
    printf("Returning to LibShell...\n");
    return 0;
}