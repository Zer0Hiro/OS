#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUFFER 100

int main()
{
    char input[BUFFER];
    char* commands[] = {"reader", "catalog", "archive"};
    int pid;
    
    // Flush buffer
    setbuf(stdout, NULL);

    // Welcome message
    printf("Welcome to libShell!\n");
    printf("Enter <reader> for reading room commands\n");
    printf("Enter <catalog> for catalog search\n");
    printf("Enter <archive> for file archive tools\n");
    printf("Lib$** ");

    while (scanf("%s", input) == 1 && strcmp(input, "exit") != 0)
    {
        int found = 0;
        // Find command from commands[]
        for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
        {
            if (strcmp(input, commands[i]) == 0)
            {
                found = 1;
                pid = fork();
                // Fork failed
                if (pid < 0)
                {
                    perror("Fork failed");
                    exit(1);
                }

                // Child
                if (pid == 0)
                {
                    char* args[] = {input, NULL};
                    execv(input, args);

                    // fails
                    perror("Exec failed");
                    exit(1);
                }
                else
                {
                    wait(NULL);
                }
            }
        }
        if (!found) printf("Not Supported\n");

        printf("Lib$** ");
    }
    printf("\nGoodbye from LibShell\n");
    return 0;
}